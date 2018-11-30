#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "comandos.h"
#include "procesamiento.h"
#include "hash.h"
#include "abb.h"
#include "strutil.h"



void liberar_vectores(char** v1, char** v2, char** v3, char** v4){
	free_strv(v1);
	free_strv(v2);
	free_strv(v3);
	free_strv(v4);
}

bool comparar_fechas(char* fecha_inicio, char* fecha_fin){

	char** anio_mes_inicio = split(fecha_inicio,'-');
	char** anio_mes_fin = split(fecha_fin, '-');
	char** dia_inicio = split(anio_mes_inicio[2], 'T');
	char** dia_fin = split(anio_mes_fin[2], 'T');
	
	char* ptr;
	
	if(strtol(anio_mes_fin[0],&ptr, 10) < strtol(anio_mes_inicio[0],&ptr, 10)){
		liberar_vectores(anio_mes_inicio, anio_mes_fin, dia_inicio, dia_fin);
		return false;
	}
	if(strtol(anio_mes_fin[0],&ptr, 10) == strtol(anio_mes_inicio[0],&ptr, 10)){
		if(strtol(anio_mes_fin[1],&ptr, 10) < strtol(anio_mes_inicio[1],&ptr, 10)){
			liberar_vectores(anio_mes_inicio, anio_mes_fin, dia_inicio, dia_fin);
			return false;
		}
			
		if(strtol(anio_mes_fin[1],&ptr, 10) == strtol(anio_mes_inicio[1],&ptr, 10)){	
		
			if(strtol(*dia_fin,&ptr, 10) < strtol(*dia_inicio,&ptr, 10)){
				liberar_vectores(anio_mes_inicio, anio_mes_fin, dia_inicio, dia_fin);
				return false;
			}
		}
	}
	liberar_vectores(anio_mes_inicio, anio_mes_fin, dia_inicio, dia_fin);

	return true;
}

bool vuelo_valido(char* linea, long int* vuelos){

	char* ptr;
	*vuelos = strtol(linea,&ptr, 10);

	if(*vuelos <= 0 || *ptr)
		return false;
	return true;
}

int cantidad_parametros(char* linea){
	
	char** vector = split(linea, ' ');
	int contador = 0, i = 0;
	
	while(vector[i] != NULL){
		contador++;
		i++;
	}
		
	free_strv(vector);
	return contador;
}

//Elimina el salto de línea para procesar las líneas

void quitar_salto_linea(char* linea){

	size_t len = strlen(linea);
	if(linea[len-1] == '\n')
		linea[len-1] = '\0';

}

void quitar_salto_en_arreglo(char** arreglo){
	for(size_t i = 0; arreglo[i]; i++)
		quitar_salto_linea(arreglo[i]);
}

bool ejecutar_comandos(char* linea[], char* linea_, hash_t* hash, abb_t* abb, char** comando){

	//Declaración de variables auxiliares e inicialización

	long int vuelos = 0;
	quitar_salto_en_arreglo(linea);

	//Ejecución del comando correspondiente

	if(!strcmp(*linea, CMD_AGREGAR_ARCHIVO)){
		*comando = strdup(CMD_AGREGAR_ARCHIVO);
		if(cantidad_parametros(linea_) != 2)
			return false;
			
		return agregar_archivo(linea[CMD_POS_ARCHIVO], hash, abb);
	}

	if(!strcmp(*linea, CMD_BORRAR_VUELOS)){
		*comando = strdup(CMD_BORRAR_VUELOS);
		if(!comparar_fechas(linea[CMD_FECHA_BORRAR_INICIO], linea[CMD_FECHA_BORRAR_FIN]))
			return false;
		if(cantidad_parametros(linea_) != 3)
			return false;
			
		return borrar(abb, hash, linea[CMD_FECHA_BORRAR_INICIO], linea[CMD_FECHA_BORRAR_FIN]);
	}

	if(!strcmp(*linea, CMD_VER_TABLERO)){
		*comando = strdup(CMD_VER_TABLERO);
		if(!vuelo_valido(linea[CMD_POS_CANT_VUELOS], &vuelos) || !comparar_fechas(linea[CMD_FECHA_VT_INICIO], linea[CMD_FECHA_VT_FIN]))
			return false;
		if(strcmp(linea[CMD_POS_MODO], MODO_ASCENDENTE) && strcmp(linea[CMD_POS_MODO],MODO_DESCENDENTE))
			return false;
		if(cantidad_parametros(linea_) != 5)
			return false;
			
		return ver_tablero(abb, vuelos, linea[CMD_FECHA_VT_INICIO], linea[CMD_FECHA_VT_FIN], linea[CMD_POS_MODO]);
	}

	if(!strcmp(*linea, CMD_INFO_VUELO)){
		*comando = strdup(CMD_INFO_VUELO);
		if(cantidad_parametros(linea_) != 2)
			return false;
			
		return ver_informacion_vuelo(hash, linea[CMD_POS_CODIGO_VUELO]);
	}
	if(!strcmp(*linea, CMD_PRIORIDAD_VUELOS)){
		*comando = strdup(CMD_PRIORIDAD_VUELOS);
		if(!vuelo_valido(linea[CMD_POS_PRIO_VUELO], &vuelos))
			return false;
		if(cantidad_parametros(linea_) != 2)
			return false;
			
		return prioridad_vuelos(hash, vuelos);	
	}
	return false;
}

