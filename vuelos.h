#ifndef VUELOS_H
#define VUELOS_H

#define POS_NUMERO_VUELO		0
#define POS_PRIORIDAD_VUELO		5
#define POS_FECHA_VUELO			6

//Si la cantidad de vuelos a imprimir es un número entero mayor a cero, devuelve true.
//Devuelve false en caso contrario.
bool cantidad_vuelos_valida(char* linea, long int* vuelos);

//Compara las fechas de inicio y fin. 
//Devuelve true en caso de que la fecha de inicio sea menor que la de fin, false en caso contrario.
bool comparar_fechas(char* fecha_inicio, char* fecha_fin);

//La función comparar_prioridades devuelve:
//-1 si la primera prioridad es mayor a la segunda.
//1 si la primera prioridad es menor a la segunda.
//0 en caso de ser iguales.
int comparar_prioridades(const void* a, const void* b);

//Devuelve un arreglo que contiene la fecha y la clave del vuelo.
char** fecha_y_clave(const char* fecha, char* vuelo);

//Devuelve un arreglo que contiene la prioridad y la clave del vuelo
char** prioridad_y_clave(char* linea);

//Destruye un arreglo que contiene la prioridad y la clave del vuelo.
void destruir_prioridad_y_clave(void* a);

//Destruye un arreglo que contiene la fecha y la clave del vuelo.
void destruir_fecha_y_clave(char*** datos_vuelos, size_t cantidad);

#endif