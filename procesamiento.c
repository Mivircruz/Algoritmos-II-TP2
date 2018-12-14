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

	if(!archivo)
		return false;

	while(!feof(archivo) && getline(&linea, &capacidad, archivo) != -1){

		info_vuelo = split(linea, ',');
		quitar_salto_en_arreglo(info_vuelo);
		char* datos_vuelo_a_guardar = join(info_vuelo, ' ');
		
		//Si el número de vuelo ya se encuentra en el sistema, actualiza su información en el abb:

		abb_guardar(abb, concatenar_cad_sep(info_vuelo[POS_FECHA_VUELO], info_vuelo[POS_NUMERO_VUELO], ' '), datos_vuelo_a_guardar);
		hash_guardar(hash, info_vuelo[POS_NUMERO_VUELO], datos_vuelo_a_guardar);
		
		free_strv(info_vuelo);
	}

	free(linea);
	fclose(archivo);
	return true;
}

bool borrar(abb_t* abb, hash_t* hash, char* fecha_desde, char* fecha_hasta){

	abb_iter_t* iter = abb_iter_in_crear(abb, fecha_desde, fecha_hasta);
	const char* clave;
	char** abb_claves = malloc(sizeof(char*)* abb_cantidad(abb));
	char* datos_vuelo;
	size_t i = 0;
	
	clave = abb_iter_in_ver_actual(iter);

	for(;!abb_iter_in_al_final(iter) && comparar_claves_abb(clave, fecha_hasta) < 0; i++){

		abb_claves[i] = strdup(clave);
		datos_vuelo = abb_obtener(abb, clave);
		printf("%s\n", datos_vuelo);

		hash_borrar(hash, datos_vuelo);

		abb_iter_in_rango_avanzar(iter);
		clave = abb_iter_in_ver_actual(iter);
	}

	abb_iter_in_destruir(iter);
	
	//Borra los vuelos correspondientes en el abb.
	for(size_t j = 0; j < i; j++){
		abb_borrar(abb, abb_claves[j]);
		free(abb_claves[j]);
	}

	return true;
}

bool ver_tablero(abb_t* abb, size_t cantidad_vuelos, char* fecha_desde, char* fecha_hasta, char* modo){
	
	
	cola_t* cola_a_imprimir = cola_crear();
	pila_t* pila_a_imprimir = pila_crear();

	abb_iter_t* iter = abb_iter_in_crear(abb, fecha_desde, fecha_hasta);
	const char* clave;
	char** datos_vuelo;
	char* a_imprimir;
	size_t j;
	
	clave = abb_iter_in_ver_actual(iter);
	
	for(j = 0; !abb_iter_in_al_final(iter) && comparar_claves_abb(clave, fecha_hasta) < 0; j++){
		
		datos_vuelo = split(clave, ' ');
		a_imprimir = concatenar_cad_sep(*datos_vuelo, datos_vuelo[1], '-');
		if(!strcmp(modo, MODO_ASCENDENTE))
			cola_encolar(cola_a_imprimir, a_imprimir);
		else
			pila_apilar(pila_a_imprimir, a_imprimir);

		abb_iter_in_rango_avanzar(iter);
		clave = abb_iter_in_ver_actual(iter);
		
	}


	for(size_t i = 0; i < j; i++){
		if(!strcmp(modo, MODO_ASCENDENTE))
			printf("%s\n",(char*)cola_desencolar(cola_a_imprimir));
		else
			printf("%s\n",(char*)pila_desapilar(pila_a_imprimir));
	}
	
	abb_iter_in_destruir(iter);
	cola_destruir(cola_a_imprimir, NULL);
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
	long int i;

	for(i = 0; i < cantidad_vuelos && !hash_iter_al_final(iter); i++){
 
		a_encolar[i] = prioridad_y_clave((char*)hash_obtener(hash, hash_iter_ver_actual(iter)));
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

		hash_iter_avanzar(iter);
	}
	for(i = 0; !heap_esta_vacio(heap); i++)
		a_imprimir[i] = *(char***)heap_desencolar(heap);
	for(i = cantidad_vuelos-1; 0 <= i; i--)
		printf("%s - %s\n", *(a_imprimir[i]), a_imprimir[i][1]);

	heap_destruir(heap, destruir_prioridad_y_clave);
	hash_iter_destruir(iter);
	free(a_encolar);
	free(a_imprimir);
	return true;
}






