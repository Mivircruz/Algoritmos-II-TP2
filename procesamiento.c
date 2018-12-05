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
		
		hash_guardar(hash, info_vuelo[POS_NUMERO_VUELO], datos_vuelo_a_guardar);
		abb_guardar(abb, info_vuelo[POS_FECHA_VUELO], datos_vuelo_a_guardar);
		free_strv(info_vuelo);
	}

	free(linea);
	fclose(archivo);
	return true;
}

bool borrar(abb_t* abb, hash_t* hash, char* fecha_desde, char* fecha_hasta){

	abb_iter_t* iter = abb_iter_in_crear(abb);
	const char* clave;
	char* a_imprimir;
	char** vector_linea;
	char* abb_claves[abb_cantidad(abb)];
	lista_t* datos_vuelo;
	size_t i = 0;
	lista_iter_t* lista_iter;
	
	clave = abb_iter_in_buscar_clave(iter, fecha_desde);

	while(clave && comparar_fechas(fecha_hasta, clave) > 0){

		abb_claves[i] = strdup(clave);
		datos_vuelo = abb_obtener(abb, clave);
		while(lista_largo(datos_vuelo)){

			lista_iter = lista_iter_crear(datos_vuelo);
			a_imprimir = lista_iter_borrar(lista_iter);
			printf("%s\n", a_imprimir);
			vector_linea = split(a_imprimir, ' ');
			hash_borrar(hash, vector_linea[POS_NUMERO_VUELO]);
			free_strv(vector_linea);
			free(a_imprimir);
			lista_iter_destruir(lista_iter);
		}
		i++;

		abb_iter_in_avanzar(iter);
		clave = abb_iter_in_ver_actual(iter);
		lista_destruir(datos_vuelo, NULL);
	}

	abb_iter_in_destruir(iter);
	
	for(size_t j = 0; j < i; j++){
		abb_borrar(abb,abb_claves[j]);
		free(abb_claves[j]);
	}

	return true;
}

bool ver_tablero(abb_t* abb, size_t cantidad_vuelos, char* fecha_desde, char* fecha_hasta, char* modo){
	
	
	abb_iter_t* iter = abb_iter_in_crear(abb);
	long int i, k = 0;
	char** datos[cantidad_vuelos];
	char* linea;
	char** vector;
	lista_t* datos_vuelo;
	lista_iter_t* lista_iter;
	const char* clave;
	
	clave = abb_iter_in_buscar_clave(iter, fecha_desde);
	
	while(clave && comparar_fechas(fecha_hasta, clave)>0 && k<cantidad_vuelos){
		
		datos_vuelo = abb_obtener(abb, clave);
		lista_iter = lista_iter_crear(datos_vuelo);
		while(!lista_iter_al_final(lista_iter)){
			
			linea = (char*)lista_iter_ver_actual(lista_iter);
			vector = split(linea, ' ');
			datos[k] = fecha_y_clave(clave, vector[POS_NUMERO_VUELO]);
			k++;
			free_strv(vector);
			lista_iter_avanzar(lista_iter);
		}
		lista_iter_destruir(lista_iter);
		abb_iter_in_avanzar(iter);
		clave = abb_iter_in_ver_actual(iter);
		
	}
	
	if(!strcmp(modo, MODO_ASCENDENTE)){
		for(i = k-1; 0 <= i; i--){
		printf("%s - %s\n", *(datos[i]), datos[i][1]);
		free(*(datos[i]));
		free(datos[i][1]);
		free(datos[i]);
		}
	}
	else{
		for(i = 0; i < k; i++){
		printf("%s - %s\n", *(datos[i]), datos[i][1]);
		free(*(datos[i]));
		free(datos[i][1]);
		free(datos[i]);
		}
	}
	
	abb_iter_in_destruir(iter);
	
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

	if(!hash_cantidad(hash))
		return false;
	
	heap_t* heap = heap_crear(comparar_prioridades);
	if(!heap)
		return NULL;
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
		if(comparar_prioridades(heap_ver_max(heap), (void**)&a_comparar) > 0){
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


