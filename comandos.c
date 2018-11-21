#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "comandos.h"
#include "strutil.h"

bool comparar_fechas(char* fecha_inicio, char* fecha_fin){

	char** anio_mes_inicio = split(fecha_inicio,'-');
	char** anio_mes_fin = split(fecha_fin, '-');

	char* ptr;
	size_t i = 0;
	for(; i < 2; i++){
		if(strtol(anio_mes_fin[i],&ptr, 10) < strtol(anio_mes_inicio[i],&ptr, 10))
			return false;
	}

	char** dia_inicio = split(anio_mes_inicio[i], 'T');
	char** dia_fin = split(anio_mes_fin[i], 'T');
	if(strtol(*dia_fin,&ptr, 10) < strtol(*dia_inicio,&ptr, 10))
		return false;

	return true;
}

bool validar_argumentos(char* argv[], comando_t* comando, long int* vuelos){	
	char* ptr;

	if(!strcmp(*argv, CMD_AGREGAR_ARCHIVO))
		*comando = AGREGAR_ARCHIVO;

	if(!strcmp(*argv, CMD_BORRAR_VUELOS)){

		if(!comparar_fechas(argv[CDM_FECHA_BORRAR_INICIO], argv[CDM_FECHA_BORRAR_FIN]))
			return false;
		*comando = BORRAR_VUELO;
	}
	else{

		if(!strcmp(*argv, CMD_VER_TABLERO)){
			*vuelos = strtol(argv[CMD_POS_CANT_VUELOS],&ptr, 10);
			if(strcmp(argv[CMD_POS_MODO], MODO_ASCENDENTE) && strcmp(argv[CMD_POS_MODO],MODO_DESCENDENTE))
				return false;
			if(!comparar_fechas(argv[CMD_FECHA_VT_INICIO], argv[CMD_FECHA_VT_FIN]))
				return false;
			*comando = VER_TABLERO;
		}

		if(!strcmp(*argv, CMD_INFORMACION_VUELO)){
			*vuelos = strtol(argv[CMD_POS_CODIGO_VUELO],&ptr, 10);
			*comando = INFORMACION_VUELO;
		}

		if(!strcmp(*argv, CMD_PRIORIDAD_VUELOS)){
			*vuelos = strtol(argv[CMD_POS_PRIO_VUELO],&ptr, 10);
			*comando = PRIORIDAD_VUELOS;
		}

		if(!comando)
			return false;

		if(*vuelos <= 0 || *ptr)
			return false;
	} 
	return true;
}
