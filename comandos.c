#include <stdio.h>
#include "comandos.h"

bool validar_argumentos(int argc, char* argv[], comando_t* comando){		

	if(!strcmp(*argv, CMD_AGREGAR_ARCHIVO)){
		if(argc != CANT_CMD_AGREGAR_ARCH)
			return false;
		*comando = AGREGAR_ARCHIVO;
	}

	else if(!strcmp(*argv, CMD_VER_TABLERO)){
		if(argc != CANT_CMD_VER_TABLERO)
			return false;

	}
	else if(!strcmp(*argv, CMD_INFORMACION_VUELO)){
		if(argc != CANT_CMD_INFO_VUELO)
			return false;

	}
	else if(!strcmp(*argv, CMD_PRIORIDAD_VUELOS)){
		if(argc != CANT_CMD_PRIO_VUELO)
			return false;

	}

	else if(!strcmp(*argv, CMD_BORRAR_VUELOS)){
		if(argc != CANT_CMD_BORRAR_VUELOS)
			return false;

	}
	else
		return false;
	return true;

	char* ptr;

	*cant_lineas = strtol(argv[ARG_CANT_LINEAS_POS],&ptr, 10);
	if(*cant_lineas <= 0 || *ptr){
		fprintf(stderr, "%s\n", MSG_ERROR_COMANDO_INCORRECTO);
		return ERROR_COMANDO_INCORRECTO;
	}

	if(argc == CANT_ARGUMENTOS_MIN)
		*archivo = false;
	else
		*archivo = true;

	return OK;

}
