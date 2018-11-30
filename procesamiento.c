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
	return 0;
}

char** fecha_y_clave(const char* fecha, char* vuelo){
	char** a_devolver = malloc(sizeof(char*) * 2);
	*a_devolver = strdup(fecha);
	a_devolver[1] = strdup(vuelo);
	
	return a_devolver;
}

char** prioridad_y_clave(char* linea){
	char** a_devolver = malloc(sizeof(char*) * 2);
	char** aux = split(linea, ' ');
	*a_devolver = strdup(aux[POS_CSV_PRIO_VUELO]);
	a_devolver[1] = strdup(aux[POS_NUMERO_VUELO]);
	free_strv(aux);
	return a_devolver;
}

void destruir_prioridad_y_clave(void* a){
	char*** aux = (char***)a;
	free(**aux);
	free((*aux)[1]);
	free(*aux);
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
		char* vuelo = join(info_vuelo, ' ');
		
		hash_guardar(hash, info_vuelo[POS_NUMERO_VUELO], vuelo);
		abb_guardar(abb, info_vuelo[POS_FECHA_VUELO], vuelo);
		free_strv(info_vuelo);
	}
	
	free(linea);
	fclose(archivo);
	return true;
}

bool ver_tablero(abb_t* abb, size_t cantidad_vuelos, char* fecha_desde, char* fecha_hasta, char* modo){
	
	abb_iter_t* iter = abb_iter_in_crear(abb);
	int i, k = 0;
	char** datos[cantidad_vuelos];
	
	for(i = 0; k<cantidad_vuelos && !abb_iter_in_al_final(iter); i++){
		const char* clave = abb_iter_in_ver_actual(iter);
		if(strcmp(clave, fecha_desde)>0 && strcmp(clave, fecha_hasta)<0){
			
			datos[k] = fecha_y_clave(clave, (char*)abb_obtener(abb, clave));
			k++;
			
		}
		abb_iter_in_avanzar(iter);
	}
	
	if(strcmp(modo, MODO_ASCENDENTE) == 0){
		for(i = 0; i < k; i++){
		printf("%s - %s\n", *(datos[i]), datos[i][1]);
		free(*(datos[i]));
		free(datos[i][1]);
		}
	}
	else{
		for(i = k-1; 0 <= i; i--){
		printf("%s - %s\n", *(datos[i]), datos[i][1]);
		free(*(datos[i]));
		free(datos[i][1]);
		}
	}
	
	abb_iter_in_destruir(iter);
	
	return true;
}

bool borrar(abb_t* abb, hash_t* hash, char* fecha_desde, char* fecha_hasta){
	
	abb_iter_t* iter = abb_iter_in_crear(abb);
	const char* clave;
	char** vector_linea;
	char** linea = malloc(sizeof(char*)*abb_cantidad(abb));
	size_t i = 0;
	
	while(!abb_iter_in_al_final(iter)){

		clave = abb_iter_in_ver_actual(iter);
		if(comparar_fechas(fecha_desde, (char*)clave) && comparar_fechas((char*)clave, fecha_hasta)){
			
			linea[i] = (char*)abb_obtener(abb, clave);
			vector_linea = split(linea[i], ' ');
			hash_borrar(hash, vector_linea[POS_NUMERO_VUELO]);
			free_strv(vector_linea);
			printf("%s\n", linea[i]);	
			i++;
		}
		abb_iter_in_avanzar(iter);
	}

	abb_iter_in_destruir(iter);

	for(size_t j = 0; j < i; j++){
		abb_borrar(abb,linea[j]);
		
	}
	free(linea);
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

