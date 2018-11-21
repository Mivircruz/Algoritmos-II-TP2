#ifndef COMANDOS_H
#define COMANDOS_H

#define CMD_AGREGAR_ARCHIVO			"agregar_archivo"
#define CMD_POS_AGREGAR_ARCHIVO		1
#define CANT_CMD_AGREGAR_ARCH		2

#define CMD_VER_TABLERO				"ver_tablero"
#define CMD_POS_CANT_VUELOS			1
#define CMD_POS_MODO				2
#define CMD_FECHA_VT_INICIO			3
#define CMD_FECHA_VT_FIN			4
#define CANT_CMD_VER_TABLERO		5
#define MODO_ASCENDENTE				"asc"
#define MODO_DESCENDENTE			"desc"

#define CMD_INFORMACION_VUELO		"info_vuelo"
#define CMD_POS_CODIGO_VUELO		1
#define CANT_CMD_INFO_VUELO			2

#define CMD_PRIORIDAD_VUELOS		"prioridad_vuelos"
#define CMD_POS_PRIO_VUELO			1
#define CANT_CMD_PRIO_VUELO			2

#define CMD_BORRAR_VUELOS			"borrar"
#define CDM_FECHA_BORRAR_INICIO		1
#define CDM_FECHA_BORRAR_FIN		2
#define CANT_CMD_BORRAR_VUELOS		3



typedef enum{
	AGREGAR_ARCHIVO,
	VER_TABLERO,
	INFORMACION_VUELO,
	PRIORIDAD_VUELOS,
	BORRAR_VUELO
}comando_t;

#endif