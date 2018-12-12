#ifndef ABB_H
#define ABB_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lista.h"

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct abb abb_t;

typedef struct abb_iter abb_iter_t;


//Compara dos cadenas.
//Pre: recibe 2 cadenas. No se modifican las cadenas recibidas.
//Post: devuelve
//*)Entero MENOR a 0 si la primer cadena es menor a la segunda
//*)Entero MAYOR a 0 si la primer cadena es mayor a la segunda
//*)Entero IGUAL a 0 si ambas cadenas son iguales
typedef int (*abb_comparar_clave_t) (const char *, const char *);


typedef void (*abb_destruir_dato_t) (void *);


/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

//Crea un abb.
//Pre: se pasa por parámetro dos funciones: una para comparar cadenas y otra, para destruir datos.
//Post: el abb fue creado. En caso de error, devuelve NULL.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

//Inserta un elemento a la izquierda o derecha según corresponda.
//Pre: el abb fue creado.
//Post: se insertó un elemento al árbol.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

//Si se quiere sacar la lista que contenga la fecha pasada por parámetro, extra = NULL. 
//Si se desea sacar una fecha con determinado número de vuelo, extra = nro de vuelo.
//En caso de no encontrarse en el abb, se deuelve NULL.
//Pre: el abb fue creado.
//Post: se devolvió el dato del elemento o NULL según corresponda.
void* abb_borrar(abb_t *arbol, const char *clave, const char* extra);

//Devuelve el elemento en caso de estar en el abb, NULL en caso contrario.
//Pre: el abb fue creado.
lista_t* abb_obtener(const abb_t *arbol, const char *clave);

//Devuelve true si el la clave se encuentra en el árbol. False en caso contrario.
//Pre: el abb fue creado.
bool abb_pertenece(const abb_t *arbol, const char *clave);

//Muestra la cantidad de elementos del árbol.
//Pre: el árbol fue creado.
size_t abb_cantidad(abb_t *arbol);

//Destruye el abb y libera la memoria pedida.
//Pre: el abb fue creado.
//Post: el abb fue destruido.
void abb_destruir(abb_t *arbol);

/* *****************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

//Recorre el abb aplicando la función visitar.
//Pre: el abb y la función visitar fueron creadas. No se puede eliminar o insterar elementos mientras se recorre el abb.
void abb_in_order(abb_t* arbol, bool visitar(const char*, lista_t*, void*), void* extra);

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

//Crea un iterador externo que recorre el arbol en el rango de las fecha pasadas y con el modo especificado.
//Pre: el arbol fue creado.
//Post: el iterador fue creado.
abb_iter_t *abb_iter_in_crear(const abb_t* arbol, char* desde, char* hasta, char* modo);

//Avanza siguiendo un recorrido que depende del modo pasado por parámetro. 
//Devuelve true si pudo avanzar, false en caso contrario.
//Pre: el iterador fue creado.
bool abb_iter_in_rango_avanzar(abb_iter_t* iter, const char* desde, const char* hasta, char* modo);

//Muestra la clave del nodo al que está apuntado.
//Pre: el iterador fue creado.
const char* abb_iter_in_ver_actual(const abb_iter_t* iter);

//Devuelve true si el iterador se encuentra al final, false en caso contrario.
//Pre: el iterador fue creado.
bool abb_iter_in_al_final(const abb_iter_t* iter);

//Destruye el iterador.
//Pre: el iterador fue creado.
//Pos: se liberó la memoria pedida para el iterador.
void abb_iter_in_destruir(abb_iter_t* iter);


#endif
