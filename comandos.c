#include <stdio.h>
#include "comandos.h"
#include "strutil.h"

bool comparar_fechas(char* fecha_inicio, char* fecha_fin){

	char** año_mes_inicio = split(fecha_inicio,'-');
	char** año_mes_fin = split(fecha_fin, '-');

	char* ptr;
	size_t i = 0;
	for(; i < 2; i++){
		if(strtol(año_mes_fin[i],&ptr, 10) < strtol(año_mes_inicio[i],&ptr, 10))
			return false;
	}

	char** dia_inicio = split(año_mes_inicio[i], 'T');
	char** dia_fin = split(año_mes_fin[i], 'T');
	if(strtol(*dia_fin,&ptr, 10) < strtol(*dia_inicio,&ptr, 10))
		return false;

	return true;
}

bool validar_argumentos(int argc, char* argv[], comando_t* comando, int* vuelos){		

	if(!strcmp(*argv, CMD_AGREGAR_ARCHIVO)){

		if(argc != CANT_CMD_AGREGAR_ARCH)
			return false;
		*comando = AGREGAR_ARCHIVO;
	}
	else if(!strcmp(*argv, CMD_BORRAR_VUELOS)){

		if(argc != CANT_CMD_BORRAR_VUELOS)
			return false;
		if(!comparar_fechas(argv[CMD_FECHA_BORRAR_INICIO], argv[CDM_FECHA_BORRAR_FIN]))
			return false;
		*comando = BORRAR_VUELO;
	}

	char* ptr;

	else{

		if(!strcmp(*argv, CMD_VER_TABLERO)){
			if(argc != CANT_CMD_VER_TABLERO)
				return false;
			*vuelos = strtol(argv[CMD_POS_CANT_VUELOS],&ptr, 10);
			if(strcmp(argv[CMD_POS_MODO], MODO_ASCENDENTE) && strcmp(argv[CMD_POS_MODO],MODO_DESCENDENTE))
				return false;
			if(!comparar_fechas(argv[CMD_FECHA_VT_INICIO], argv[CMD_FECHA_VT_FIN]))
				return false;
			*comando = VER_TABLERO;
		}

		else if(!strcmp(*argv, CMD_INFORMACION_VUELO)){
			if(argc != CANT_CMD_INFO_VUELO)
				return false;
			*vuelos = strtol(argv[CMD_POS_CODIGO_VUELO],&ptr, 10);
			*comando = INFORMACION_VUELO;
		}
		else if(!strcmp(*argv, CMD_PRIORIDAD_VUELOS)){
			if(argc != CANT_CMD_PRIO_VUELO)
				return false;
			*vuelos = strtol(argv[CMD_POS_PRIO_VUELO],&ptr, 10);
			*comando = PRIORIDAD_VUELOS;
		}

		if(*vuelos <= 0 || *ptr)
			return false;
	} 
	return true;
}
