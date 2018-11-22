#ifndef PROCESAMIENTO_H
#define PROCESAMIENTO_H

#define POS_NUMERO_VUELO		0
#define POS_FECHA_VUELO			6

//Procesa el archivo con fornato csv y guarda la información de los vuelos.
void agregar_archivo(FILE* archivo,hash_t* hash, abb_t* abb);


//En caso de encontrar el código de vuelo, devuelve true.
//En caso contrario, devuelve false.
bool ver_informacion_vuelo(hash_t* hash, char* codigo_vuelo, char** informacion);

#endif