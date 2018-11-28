#ifndef PROCESAMIENTO_H
#define PROCESAMIENTO_H

#define POS_NUMERO_VUELO		0
#define POS_FECHA_VUELO			6

//Procesa el archivo con formato csv y guarda la información de los vuelos.
bool agregar_archivo(char* nombre_archivo, hash_t* hash, abb_t* abb);


//En caso de encontrar el código de vuelo, devuelve true.
//En caso contrario, devuelve false.
bool ver_informacion_vuelo(hash_t* hash, char* codigo_vuelo);

//Imprime una cantidad de vuelos pasada por parámetro según su prioridad.
bool prioridad_vuelos(hash_t* hash, size_t cantidad_vuelos);

//Imprime una cantidad de vuelos comprendidos entre las fechas indicadas.
bool ver_tablero(abb_t* abb, size_t cantidad_vuelos, char* fecha_desde, char* fecha_hasta, char* modo);

//Elimina los vuelos comprendidos entre las fechas indicadas, y los muestra por pantalla.
bool borrar(abb_t* abb, hash_t* hash, char* fecha_desde, char* fecha_hasta);

#endif
