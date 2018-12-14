#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "comandos.h"
#include "strutil.h"
#include "vuelos.h"

#define POS_ANIO	0
#define POS_MES		1
#define POS_DIA		2
#define POS_HORA	0
#define POS_MIN		1
#define POS_SEG		2

/* ******************************************************************
 *                        FUNCIONES AUXILIARES
 * *****************************************************************/

//Libera la memoria pedida para comparar fechas en la función "comparar fechas"
void liberar_vectores(char** v1, char** v2, char** v3, char** v4, char** v5, char** v6){
	
	if(v1)
		free_strv(v1);
	if(v2)
		free_strv(v2);
	if(v3)
		free_strv(v3);
	if(v4)
		free_strv(v4);
	if(v5)
		free_strv(v5);
	if(v6)
		free_strv(v6);
}

//Parsea las cadenas pasadas a enteros. Devuelve
// a < b: -1
// a > b: 1
// a = b: 0
int comparar_cadenas_a_numeros(char* a, char* b){

	char* ptr;

	long int numero1 = strtol(a,&ptr, 10);
	long int numero2 = strtol(b,&ptr, 10);

	if(numero1 < numero2)
		return -1;
	else if(numero1 > numero2)
		return 1;
	else
		return 0;
}

/* ******************************************************************
 *                        FUNCIONES PRINCIPALES
 * *****************************************************************/

int comparar_fechas(const char* fecha_inicio, const char* fecha_fin){

	char** anio_mes_inicio = split(fecha_inicio,'-');
	char** anio_mes_fin = split(fecha_fin, '-');

	int comparacion_anio, comparacion_mes, comparacion_dia, comparacion_hora, comparacion_min, comparacion_seg;
	
	comparacion_anio = comparar_cadenas_a_numeros(anio_mes_fin[POS_ANIO],anio_mes_inicio[POS_ANIO]);
	if(comparacion_anio < 0){
		liberar_vectores(anio_mes_inicio, anio_mes_fin, NULL, NULL, NULL, NULL);
		return 1;
	}
	if(!comparacion_anio){

		char** dia_inicio = split(anio_mes_inicio[POS_DIA], 'T');
		char** dia_fin = split(anio_mes_fin[POS_DIA], 'T');

		comparacion_mes = comparar_cadenas_a_numeros(anio_mes_fin[POS_MES],anio_mes_inicio[POS_MES]);
		if(comparacion_mes < 0){
			liberar_vectores(anio_mes_inicio, anio_mes_fin, dia_inicio, dia_fin, NULL, NULL);
			return 1;
		}
			
		if(!comparacion_mes){	
		
			comparacion_dia = comparar_cadenas_a_numeros(*dia_fin, *dia_inicio);
			if(comparacion_dia < 0){
				liberar_vectores(anio_mes_inicio, anio_mes_fin, dia_inicio, dia_fin, NULL, NULL);
				return 1;
			}
			if(!comparacion_dia){

				char** tiempo_inicio = split(dia_inicio[1],':');
				char** tiempo_fin = split(dia_fin[1],':');

				comparacion_hora = comparar_cadenas_a_numeros(tiempo_fin[POS_HORA], tiempo_inicio[POS_HORA]);
				if(comparacion_hora < 0){
					liberar_vectores(anio_mes_inicio, anio_mes_fin, dia_inicio, dia_fin, tiempo_inicio, tiempo_fin);
					return 1;
				}
				if(!comparacion_hora){
					comparacion_min = comparar_cadenas_a_numeros(tiempo_fin[POS_MIN], tiempo_inicio[POS_MIN]);
					if(comparacion_min < 0){
						liberar_vectores(anio_mes_inicio, anio_mes_fin, dia_inicio, dia_fin, tiempo_inicio, tiempo_fin);
						return 1;
					}
					if(!comparacion_min){

						comparacion_seg = comparar_cadenas_a_numeros(tiempo_fin[POS_SEG], tiempo_inicio[POS_SEG]);
						if(comparacion_seg < 0){
							liberar_vectores(anio_mes_inicio, anio_mes_fin, dia_inicio, dia_fin, tiempo_inicio, tiempo_fin);
							return 1;
						}
						if(!comparacion_seg){
							liberar_vectores(anio_mes_inicio, anio_mes_fin, dia_inicio, dia_fin, tiempo_inicio, tiempo_fin);
							return 0;
						}
					}
				}
				liberar_vectores(tiempo_inicio, tiempo_fin, NULL, NULL, NULL, NULL);
			}
		}

		liberar_vectores(dia_inicio, dia_fin, NULL, NULL, NULL, NULL);
	}
	liberar_vectores(anio_mes_inicio, anio_mes_fin, NULL, NULL, NULL, NULL);
	return -1;
}

int comparar_claves_abb(const char* clave1, const char* clave2){

	char** fecha_y_clave_1 = split(clave1, '-');
	char** fecha_y_clave_2 = split(clave2, '-');
	int comp = comparar_fechas(*fecha_y_clave_1, *fecha_y_clave_2);
	if(!comp)
		comp = strcmp(fecha_y_clave_1[1], fecha_y_clave_2[1]);

	free_strv(fecha_y_clave_1);
	free_strv(fecha_y_clave_2);

	return comp;
}

bool cantidad_vuelos_valida(char* linea, long int* vuelos){

	char* ptr;
	*vuelos = strtol(linea,&ptr, 10);

	if(*vuelos <= 0 || *ptr)
		return false;
	return true;
}

int comparar_prioridades(const void* a, const void* b){

	char* ptr;
	char** vuelo1 = *(char***)a;
	char**vuelo2 = *(char***)b;
	long int prioridad1 = strtol(*vuelo1,&ptr,10);
	long int prioridad2 = strtol(*vuelo2,&ptr,10);
	long int codigo_vuelo1 = strtol(vuelo1[1],&ptr,10);
	long int codigo_vuelo2 = strtol(vuelo2[1],&ptr,10);
	if(prioridad1 > prioridad2)
		return -1;
	if(prioridad1 < prioridad2)
		return 1;
	if(prioridad1 == prioridad2){
		if(codigo_vuelo2 < codigo_vuelo1)
			return 0;
		else
			return -1;
	}
	return 0;
}

char** fecha_y_clave(const char* fecha, char* vuelo){
	char** a_devolver = malloc(sizeof(char*) * 2);
	*a_devolver = strdup(fecha);
	a_devolver[1] = strdup(vuelo);
	
	return a_devolver;
}

void swap_datos_vuelo(char*** x, char*** y) {

	char** aux;

	if(*x == *y)
		return;

	aux = *x;
	*x = *y;
	*y = aux;
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

void destruir_fecha_y_clave(char*** datos_vuelos, size_t cantidad){

	for(size_t i = 0; i < cantidad; i++){
		free(*(datos_vuelos[i]));
		free(datos_vuelos[i][1]);
		free(datos_vuelos[i]);
	}
}

