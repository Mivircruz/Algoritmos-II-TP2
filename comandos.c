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

bool vuelo_valido(char* linea, long int* vuelos){

	char* ptr;
	*vuelos = strtol(linea,&ptr, 10);

	if(*vuelos <= 0 || *ptr)
		return false;
	return true;
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

bool ejecutar_comandos(char* linea[], hash_t* hash, abb_t* abb, char** comando){

	//Declaración de variables auxiliares e inicialización

	long int* vuelos = 0;
	quitar_salto_en_arreglo(linea);

	//Ejecución del comando correspondiente

	if(!strcmp(*linea, CMD_AGREGAR_ARCHIVO)){
		*comando = strdup(CMD_AGREGAR_ARCHIVO);
		return agregar_archivo(linea[CMD_POS_ARCHIVO], hash, abb);
	}

	if(!strcmp(*linea, CMD_BORRAR_VUELOS)){
		*comando = strdup(CMD_BORRAR_VUELOS);
		if(!comparar_fechas(linea[CDM_FECHA_BORRAR_INICIO], linea[CDM_FECHA_BORRAR_FIN]))
			return false;
	
	//FALTA FUNCIÓN BORRAR 				!!!!!!!!
	}

	if(!strcmp(*linea, CMD_VER_TABLERO)){
		*comando = strdup(CMD_VER_TABLERO);
		if(!vuelo_valido(linea[CMD_POS_CANT_VUELOS], vuelos) || !comparar_fechas(linea[CMD_FECHA_VT_INICIO], linea[CMD_FECHA_VT_FIN]))
			return false;
		if(strcmp(linea[CMD_POS_MODO], MODO_ASCENDENTE) && strcmp(linea[CMD_POS_MODO],MODO_DESCENDENTE))
			return false;
	//FALTA FUNCIÓN VER TABLERO 				!!!!!!!!
	}

	if(!strcmp(*linea, CMD_INFO_VUELO)){
		*comando = strdup(CMD_INFO_VUELO);
		return ver_informacion_vuelo(hash, linea[CMD_POS_CODIGO_VUELO]);
	}
	if(!strcmp(*linea, CMD_PRIORIDAD_VUELOS)){
		*comando = strdup(CMD_PRIORIDAD_VUELOS);
		if(!vuelo_valido(linea[CMD_POS_PRIO_VUELO], vuelos))
			return false;
		return prioridad_vuelos(hash, *vuelos);	
	}
	return false;
}
