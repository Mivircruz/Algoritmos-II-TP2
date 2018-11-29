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

/* ******************************************************************
 *                        FUNCIONES AUXILIARES
 * *****************************************************************/

//La función comparar_prioridades devuelve:
//-1 si el primer entero es mayor al segundo.
//1 si el primer elemento es menor al segundo.
//0 en caso de ser iguales.

int comparar_prioridades(const void* a, const void* b){

	char* ptr;
	char** vuelo1 = *(char***)a;
	char**vuelo2 = *(char***)b;
	long int prioridad1 = strtol(*vuelo1,&ptr,10);
	long int prioridad2 = strtol(*vuelo2,&ptr,10);
	if(prioridad1 > prioridad2)
		return -1;
	if(prioridad1 < prioridad2)
		return 1;
	printf("prioridad 1 = %li, prioridad 2 = %li\n",prioridad1, prioridad2 );
	return 0;
}

char** prioridad_y_clave(char* linea){
	char** a_devolver = malloc(sizeof(char*) * 2);
	char** aux = split(linea, ' ');
	*a_devolver = strdup(aux[POS_PRIORIDAD_VUELO]);
	a_devolver[1] = strdup(aux[POS_NUMERO_VUELO]);
	free_strv(aux);
	return a_devolver;
}


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

		hash_guardar(hash, info_vuelo[POS_NUMERO_VUELO], join(info_vuelo, ' '));
		abb_guardar(abb, info_vuelo[POS_NUMERO_VUELO], info_vuelo[POS_FECHA_VUELO]);

	}
	fclose(archivo);
	return true;
}

bool ver_tablero(abb_t* abb, size_t cantidad_vuelos, char* fecha_desde, char* fecha_hasta, char* modo){
	
	abb_iter_t* iter = abb_iter_in_crear(abb);
	size_t guardados = 0;
	
	for(size_t i = 0; guardados<cantidad_vuelos && !abb_iter_in_al_final(iter); i++){
		const char* clave = abb_iter_in_ver_actual(iter);
		if(strcmp(clave, fecha_desde)>0 && strcmp(clave, fecha_hasta)<0){
			
			char** vector_linea = split((char*)abb_obtener(abb, clave), ' ');
			printf("\n%s\t%s", vector_linea[POS_FECHA_VUELO], vector_linea[POS_NUMERO_VUELO]);

			guardados++;
			free_strv(vector_linea);
		}
			
		abb_iter_in_avanzar(iter);
	}
	abb_iter_in_destruir(iter);
	
	return true;
}

bool borrar(abb_t* abb, hash_t* hash, char* fecha_desde, char* fecha_hasta){
	
	abb_iter_t* iter = abb_iter_in_crear(abb);
	
	for(size_t i =0; !abb_iter_in_al_final(iter); i++){
		
		const char* clave = abb_iter_in_ver_actual(iter);
		if(strcmp(clave, fecha_desde)>0 && strcmp(clave, fecha_hasta)<0){
			
			char* linea = (char*)abb_borrar(abb, clave);
			char** vector_linea = split(linea, ' ');
			hash_borrar(hash, vector_linea[POS_NUMERO_VUELO]);
			printf("\n%s", linea);
		}
		
		abb_iter_in_avanzar(iter);
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

	heap_t* heap = heap_crear(comparar_prioridades);
	hash_iter_t* iter = hash_iter_crear(hash);
	char* clave_hash;
	char** clave_heap;

	for(size_t i = 0; i < cantidad_vuelos; i++){

		clave_hash = (char*)hash_obtener(hash, hash_iter_ver_actual(iter));
		clave_heap = prioridad_y_clave(clave_hash);
		heap_encolar(heap, &clave_heap);
		hash_iter_avanzar(iter);
	}
	while(!hash_iter_al_final(iter)){
		clave_hash = (char*)hash_obtener(hash, hash_iter_ver_actual(iter));
		clave_heap = prioridad_y_clave(clave_hash);
		if(comparar_prioridades(heap_ver_max(heap), (void**)&clave_heap) < 0){
			heap_desencolar(heap);
			heap_encolar(heap, &(clave_heap));
		}
		hash_iter_avanzar(iter);
	}
	while(!heap_esta_vacio(heap)){
		clave_heap = *(char***)heap_desencolar(heap);
		printf("%s - %s\n",*clave_heap, clave_heap[1]);
	}
	heap_destruir(heap, free);
	return true;
}
