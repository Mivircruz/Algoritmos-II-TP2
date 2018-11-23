#ifndef COMANDOS_H
#define COMANDOS_H

#define CMD_AGREGAR_ARCHIVO			"agregar_archivo"
#define CMD_POS_AGREGAR_ARCHIVO		1

#define CMD_VER_TABLERO				"ver_tablero"
#define CMD_POS_CANT_VUELOS			1
#define CMD_POS_MODO				2
#define CMD_FECHA_VT_INICIO			3
#define CMD_FECHA_VT_FIN			4
#define MODO_ASCENDENTE				"asc"
#define MODO_DESCENDENTE			"desc"

#define CMD_INFORMACION_VUELO		"info_vuelo"
#define CMD_POS_CODIGO_VUELO		1

#define CMD_PRIORIDAD_VUELOS		"prioridad_vuelos"
#define CMD_POS_PRIO_VUELO			1
#define POS_CSV_PRIO_VUELO			5

#define CMD_BORRAR_VUELOS			"borrar"
#define CDM_FECHA_BORRAR_INICIO		1
#define CDM_FECHA_BORRAR_FIN		2

#define CANT_DATOS_VUELO			10

typedef enum{
	SIN_COMANDO = 0,
	AGREGAR_ARCHIVO,
	VER_TABLERO,
	INFORMACION_VUELO,
	PRIORIDAD_VUELOS,
	BORRAR_VUELO
}comando_t;

bool validar_argumentos(char* linea[], comando_t* comando, long int* vuelos);

#endif