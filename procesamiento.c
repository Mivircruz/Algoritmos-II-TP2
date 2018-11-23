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
	long int prioridad1 = strtol(vuelo1[CMD_POS_PRIO_VUELO],&ptr,10);
	long int prioridad2 = strtol(vuelo2[CMD_POS_PRIO_VUELO],&ptr,10);
	if(prioridad1 > prioridad2)
		return -1;
	if(prioridad1 < prioridad2)
		return 1;
	return 0;
}

/* ******************************************************************
 *                        FUNCIONES PRINCIPALES
 * *****************************************************************/

bool agregar_archivo(FILE* archivo, hash_t* hash, abb_t* abb){

	char* linea = NULL;
	size_t capacidad = 0;
	char** info_vuelo;

	while(!feof(archivo)){

		if(getline(&linea, &capacidad, archivo) == -1)
			break;

		info_vuelo = split(linea, ',');

		hash_guardar(hash, info_vuelo[POS_NUMERO_VUELO], join(info_vuelo, ' '));
		abb_guardar(abb, info_vuelo[POS_NUMERO_VUELO], info_vuelo[POS_FECHA_VUELO]);

	}
	return true;
}

bool ver_informacion_vuelo(hash_t* hash, char* codigo_vuelo, char** informacion){

	void* info_vuelo = hash_obtener(hash, codigo_vuelo);
	if(!info_vuelo)
		return false;
	*informacion = (char*)info_vuelo;
	return true;
}

bool prioridad_vuelos(hash_t* hash, size_t cantidad_vuelos){

	heap_t* heap = heap_crear(comparar_prioridades);
	hash_iter_t* iter = hash_iter_crear(hash);
	char* info_vuelo;
	char*** prioridad_y_clave = malloc(sizeof(char**) * cantidad_vuelos);
	size_t cant_encolada = 0;

	for(size_t i = 0 ; !hash_iter_al_final(iter); cant_encolada++, i++){

		info_vuelo = (char*)hash_obtener(hash, hash_iter_ver_actual(iter));
		prioridad_y_clave[i] = split(info_vuelo, ' ');
		for(size_t i = 1; i < CANT_DATOS_VUELO; i++){
			if(i == POS_CSV_PRIO_VUELO)
				continue;
			free(prioridad_y_clave[i]);
		}
		if(!heap_encolar(heap, &(prioridad_y_clave[i])))
			return false;
		hash_iter_avanzar(iter);
	}
	return true;
}
