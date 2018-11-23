 #define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "comandos.h"
#include "strutil.h"


//Compara las fechas de inicio y fin. 
//Devuelve true en caso de que la fecha de inicio sea menor que la de fin, false en caso contrario.

bool comparar_fechas(char* fecha_inicio, char* fecha_fin){

	char** anio_mes_inicio = split(fecha_inicio,'-');
	char** anio_mes_fin = split(fecha_fin, '-');

	char* ptr;
	
	if(strtol(anio_mes_fin[0],&ptr, 10) < strtol(anio_mes_inicio[0],&ptr, 10))
		return false;
	if(strtol(anio_mes_fin[0],&ptr, 10) == strtol(anio_mes_inicio[0],&ptr, 10)){
		if(strtol(anio_mes_fin[1],&ptr, 10) < strtol(anio_mes_inicio[1],&ptr, 10))
			return false;
			
		if(strtol(anio_mes_fin[1],&ptr, 10) == strtol(anio_mes_inicio[1],&ptr, 10)){	
			char** dia_inicio = split(anio_mes_inicio[2], 'T');
			char** dia_fin = split(anio_mes_fin[2], 'T');
			if(strtol(*dia_fin,&ptr, 10) < strtol(*dia_inicio,&ptr, 10))
				return false;
		}
	}
	return true;
}

//Elimina el salto de línea para procesar las líneas

void quitar_salto_linea(char* linea){

	size_t len = strlen(linea);
	if(linea[len-1] == '\n')
		linea[len-1] = '\0';

}

//Valida los comandos pasados por parámetro

bool validar_argumentos(char* linea[], char** comando, long int* vuelos){	
	
	char* ptr;

	for(size_t i = 0; linea[i]; i++)
		quitar_salto_linea(linea[i]);

	if(!strcmp(*linea, CMD_AGREGAR_ARCHIVO))
		*comando = strdup(CMD_AGREGAR_ARCHIVO);

	if(!strcmp(*linea, CMD_BORRAR_VUELOS)){

		*comando = strdup(CMD_BORRAR_VUELOS);
		if(!comparar_fechas(linea[CDM_FECHA_BORRAR_INICIO], linea[CDM_FECHA_BORRAR_FIN]))
			return false;
	}
	else{

		if(!strcmp(*linea, CMD_VER_TABLERO)){

			*comando = strdup(CMD_VER_TABLERO);
			*vuelos = strtol(linea[CMD_POS_CANT_VUELOS],&ptr, 10);
			if(strcmp(linea[CMD_POS_MODO], MODO_ASCENDENTE) && strcmp(linea[CMD_POS_MODO],MODO_DESCENDENTE))
				return false;
			if(!comparar_fechas(linea[CMD_FECHA_VT_INICIO], linea[CMD_FECHA_VT_FIN]))
				return false;
		}

		if(!strcmp(*linea, CMD_INFO_VUELO)){

			*comando = strdup(CMD_INFO_VUELO);
			*vuelos = strtol(linea[CMD_POS_CODIGO_VUELO],&ptr, 10);
		}

		if(!strcmp(*linea, CMD_PRIORIDAD_VUELOS)){

			*comando = strdup(CMD_PRIORIDAD_VUELOS);
			*vuelos = strtol(linea[CMD_POS_PRIO_VUELO], &ptr, 10);
		}
		if(*vuelos <= 0 || *ptr){
			return false;
		}
	} 
	return true;
}
