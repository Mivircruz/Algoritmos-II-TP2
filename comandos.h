#ifndef COMANDOS_H
#define COMANDOS_H

#include "hash.h"
#include "abb.h"
		
#define CMD_AGREGAR_ARCHIVO			"agregar_archivo"
#define CMD_POS_ARCHIVO				1
				
#define CMD_VER_TABLERO				"ver_tablero"
#define CMD_POS_CANT_VUELOS			1
#define CMD_POS_MODO				2
#define CMD_FECHA_VT_INICIO			3
#define CMD_FECHA_VT_FIN			4
#define MODO_ASCENDENTE				"asc"
#define MODO_DESCENDENTE			"desc"

#define CMD_INFO_VUELO				"info_vuelo"
#define CMD_POS_CODIGO_VUELO		1

#define CMD_PRIORIDAD_VUELOS		"prioridad_vuelos"
#define CMD_POS_PRIO_VUELO			1
#define POS_CSV_PRIO_VUELO			5

#define CMD_BORRAR_VUELOS			"borrar"
#define CMD_FECHA_BORRAR_INICIO		1
#define CMD_FECHA_BORRAR_FIN		2

#define CANT_DATOS_VUELO			10


//Valida los comandos pasados por parámetro y ejecuta la función correspondiente
bool ejecutar_comandos(char* linea[], char* linea_, hash_t* hash, abb_t* abb,char** comando);

//Si la cantidad de vuelos a imprimir es un número entero mayor a cero, devuelve true.
//Devuelve false en caso contrario.
bool vuelo_valido(char* linea, long int* vuelos);

//Quita el '\n' de la línea
void quitar_salto_en_arreglo(char** arreglo);

//Compara las fechas de inicio y fin. 
//Devuelve true en caso de que la fecha de inicio sea menor que la de fin, false en caso contrario.
bool comparar_fechas(char* fecha_inicio, char* fecha_fin);

#endif
