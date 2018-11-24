#ifndef PROCESAMIENTO_H
#define PROCESAMIENTO_H

#define POS_NUMERO_VUELO		0
#define POS_FECHA_VUELO			6

//Procesa el archivo con formato csv y guarda la información de los vuelos.
bool agregar_archivo(char* nombre_archivo, hash_t* hash, abb_t* abb);


//En caso de encontrar el código de vuelo, devuelve true.
//En caso contrario, devuelve false.
bool ver_informacion_vuelo(hash_t* hash, long int* codigo_vuelo, char* informacion);

//Imprime una cantidad de vuelos pasada por parámetro según su prioridad.
bool prioridad_vuelos(hash_t* hash, size_t cantidad_vuelos);

#endif
