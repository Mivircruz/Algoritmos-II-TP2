#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "hash.h"
#include "strutil.h"
#include "procesamiento.h"
#include "heap.h"
#include "comandos.h"
#include "vuelos.h"
#include "pila.h"
#include "cola.h"


/* ******************************************************************
 *                        FUNCIONES PRINCIPALES
 * *****************************************************************/

bool agregar_archivo(char* nombre_archivo, hash_t* hash, abb_t* abb){

	char* linea = NULL;
	size_t capacidad = 0;
	char** info_vuelo;
	FILE* archivo = fopen(nombre_archivo, "r");
	char* abb_clave;
	char* datos_vuelo_a_guardar;
	
	if(!archivo)
		return false;

	while(!feof(archivo) && getline(&linea, &capacidad, archivo) != -1){

		info_vuelo = split(linea, ',');
		quitar_salto_en_arreglo(info_vuelo);
		datos_vuelo_a_guardar = join(info_vuelo, ' ');
		abb_clave = concatenar_cad_sep(info_vuelo[POS_FECHA_VUELO], info_vuelo[POS_NUMERO_VUELO], " ");
		
		//Si el número de vuelo ya se encuentra en el sistema, actualiza su información en el abb:

		if(hash_pertenece(hash, info_vuelo[POS_NUMERO_VUELO])){
			char* abb_clave_vieja = (char*)hash_obtener(hash, info_vuelo[POS_NUMERO_VUELO]);
			char** datos_vuelo_viejos = split(abb_clave_vieja, ' ');
			abb_clave_vieja = concatenar_cad_sep(datos_vuelo_viejos[POS_FECHA_VUELO], datos_vuelo_viejos[POS_NUMERO_VUELO]," ");
			free(abb_borrar(abb, abb_clave_vieja));
			free_strv(datos_vuelo_viejos);
			free(abb_clave_vieja);
		}
		hash_guardar(hash, info_vuelo[POS_NUMERO_VUELO], datos_vuelo_a_guardar);
		abb_guardar(abb, abb_clave, datos_vuelo_a_guardar);
		
		free(abb_clave);
		free_strv(info_vuelo);
	}
	
	free(linea);
	fclose(archivo);
	return true;
}

bool borrar(abb_t* abb, hash_t* hash, char* fecha_desde, char* fecha_hasta){

	abb_iter_t* iter = abb_iter_in_crear(abb, fecha_desde, fecha_hasta);
	const char* clave;
	cola_t* abb_claves = cola_crear();
	char** hash_info_vuelo;
	char* datos_vuelo;
	size_t i = 0;
	
	clave = abb_iter_in_ver_actual(iter);

	for(;!abb_iter_in_al_final(iter) && comparar_claves_abb(clave, fecha_hasta) < 0; i++){

		if(comparar_claves_abb(clave, fecha_desde) < 0){
			abb_iter_in_rango_avanzar(iter);
			clave = abb_iter_in_ver_actual(iter);
			continue;
		}
		
		cola_encolar(abb_claves, strdup(clave));
		abb_iter_in_rango_avanzar(iter);
		clave = abb_iter_in_ver_actual(iter);
	}
	
	abb_iter_in_destruir(iter);
	
	while(!cola_esta_vacia(abb_claves)){
		
		clave = cola_desencolar(abb_claves);
		datos_vuelo = (char*)abb_obtener(abb, clave);
		
		printf("%s\n", datos_vuelo);
		
		//Borra los vuelos correspondientes del hash.
		hash_info_vuelo = split(datos_vuelo, ' ');
		hash_borrar(hash, hash_info_vuelo[POS_NUMERO_VUELO]);
		free(abb_borrar(abb, clave));
		free((char*)clave);
		free_strv(hash_info_vuelo);
	}

	cola_destruir(abb_claves, NULL);

	return true;
}

bool ver_tablero(abb_t* abb, size_t cantidad_vuelos, char* fecha_desde, char* fecha_hasta, char* modo){
	
	
	cola_t* cola_a_imprimir = cola_crear();
	pila_t* pila_a_imprimir = pila_crear();

	abb_iter_t* iter = abb_iter_in_crear(abb, fecha_desde, fecha_hasta);
	const char* clave;
	char** datos_vuelo;
	char* a_imprimir;
	
	clave = abb_iter_in_ver_actual(iter);
	
	for(size_t j = 0; !abb_iter_in_al_final(iter) && comparar_claves_abb(clave, fecha_hasta) < 0; j++){

		if(comparar_claves_abb(clave, fecha_desde) < 0){
			abb_iter_in_rango_avanzar(iter);
			clave = abb_iter_in_ver_actual(iter);
			continue;
		}

		datos_vuelo = split(clave, ' ');
		a_imprimir = concatenar_cad_sep(*datos_vuelo, datos_vuelo[1], " - ");
		if(!strcmp(modo, MODO_ASCENDENTE))
			cola_encolar(cola_a_imprimir, a_imprimir);
		else
			pila_apilar(pila_a_imprimir, a_imprimir);

		abb_iter_in_rango_avanzar(iter);
		clave = abb_iter_in_ver_actual(iter);
		free_strv(datos_vuelo);
	}

	for(size_t i = 0; i < cantidad_vuelos; i++){
		if(!strcmp(modo, MODO_ASCENDENTE)){
			if(!cola_esta_vacia(cola_a_imprimir)){
				char* desencolado = (char*)cola_desencolar(cola_a_imprimir);
				printf("%s\n", desencolado);
				free(desencolado);
			}
		}
		else{
			if(!pila_esta_vacia(pila_a_imprimir)){
				char* desapilado = (char*)pila_desapilar(pila_a_imprimir);
				printf("%s\n", desapilado);
				free(desapilado);		
			}
		}
	}
	
	abb_iter_in_destruir(iter);
	cola_destruir(cola_a_imprimir, free);
	pila_destruir(pila_a_imprimir);

	return true;
}

bool ver_informacion_vuelo(hash_t* hash, char* codigo_vuelo){
	
	void* info_vuelo = hash_obtener(hash, codigo_vuelo);
	if(!info_vuelo)
		return false;
	printf("%s\n",(char*)info_vuelo);
	return true;
}

bool prioridad_vuelos(hash_t* hash, size_t cantidad_vuelos){

	heap_t* heap = heap_crear(comparar_prioridades);
	if(!heap)
		return false;

	hash_iter_t* iter = hash_iter_crear(hash);
	if(!iter){
		heap_destruir(heap, NULL);
		return false;
	}

	if(cantidad_vuelos > hash_cantidad(hash))
		cantidad_vuelos = hash_cantidad(hash);

	char*** a_encolar = malloc(sizeof(char**)*cantidad_vuelos);
	char*** a_imprimir = malloc(sizeof(char**)*cantidad_vuelos);
	char** a_comparar;
	char*** reemplazante;
	long int i, contador = 0;

	for(i = 0; i < cantidad_vuelos && !hash_iter_al_final(iter); i++){
 
		a_encolar[i] = prioridad_y_clave((char*)hash_obtener(hash, hash_iter_ver_actual(iter)));
		contador++;
		heap_encolar(heap, &(a_encolar[i]));
		hash_iter_avanzar(iter);
	}
	while(!hash_iter_al_final(iter)){
		
		a_comparar = prioridad_y_clave((char*)hash_obtener(hash, hash_iter_ver_actual(iter)));
		if(comparar_prioridades(heap_ver_max(heap), (void**)&a_comparar) >= 0){
			reemplazante = heap_desencolar(heap);
			free(**reemplazante);
			free((*reemplazante)[1]);
			**reemplazante = *a_comparar;
			(*reemplazante)[1] = a_comparar[1];
			heap_encolar(heap, reemplazante);

		}
		else
			free_strv(a_comparar);
			
		hash_iter_avanzar(iter);

	}
	
	for(i = 0; !heap_esta_vacio(heap); i++)
		a_imprimir[i] = *(char***)heap_desencolar(heap);
	
	
	for(i = cantidad_vuelos-1; 0 <= i; i--){
		printf("%s - %s\n", *(a_imprimir[i]), a_imprimir[i][1]);
		
	}
	
	heap_destruir(heap, destruir_prioridad_y_clave);
	hash_iter_destruir(iter);
	for(i = 0; i < contador; i++){
		free_strv(a_encolar[i]);
	}
	free(a_encolar);
	free(a_imprimir);
	return true;
}

