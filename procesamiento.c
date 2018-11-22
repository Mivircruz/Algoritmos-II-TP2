#include <stdio.h>
#include <stdlib.h>
#include "abb.h"
#include "hash.h"
#include "strutil.h"
#include "procesamiento.h"

void agregar_archivo(FILE* archivo, hash_t* hash, abb_t* abb){

	char* linea = NULL;
	size_t capacidad = 0;
	char** info_vuelo;
	if(!hash && !abb){
		hash_t* tabla_aux = hash_crear(NULL);
		abb_t* tabla_ppal = abb_crear(strcmp, NULL);
	}

	while(!feof(archivo)){

		if(getline(&linea, &capacidad, archivo) == -1)
			break;

		info_vuelo = split(linea, ',');

		hash_guardar(hash, join(info_vuelo), info_vuelo[POS_NUMERO_VUELO]);
		abb_guardar(abb, info_vuelo[POS_FECHA_VUELO],info_vuelo[POS_NUMERO_VUELO]);

	}
}

bool ver_informacion_vuelo(hash_t* hash, char* codigo_vuelo, char** informacion){

	void* info_vuelo = hash_obtener(hash, codigo_vuelo);
	if(!info_vuelo)
		return false;
	*informacion = (char*)info_vuelo;
	return true;
}