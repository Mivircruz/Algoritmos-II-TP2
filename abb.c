#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "abb.h"
#include "lista.h"
#include "pila.h"

/* ******************************************************************
 *              DEFINICIÓN DEL STRUCT ABB E ITERADOR
 * *****************************************************************/

typedef struct nodo{

    char* clave;
    lista_t* lista;
    struct nodo* izq;
    struct nodo* der;
}nodo_t;

struct abb{

	nodo_t* raiz;
	abb_comparar_clave_t comparar_clave;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
};

struct abb_iter{

	pila_t* pila;
	abb_comparar_clave_t comparar_clave;
};

/* ******************************************************************
 *                   PRIMITIVAS DEL NODO
 * *****************************************************************/
nodo_t* nodo_crear(const char* clave, void* dato){

	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo)
		return NULL;
	nodo->lista = lista_crear();
	if(!nodo->lista){
		free(nodo);
		return NULL;
	}

	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->clave = strdup(clave);
	lista_insertar_ultimo(nodo->lista, dato);
	return nodo;
}

/* ******************************************************************
 *                   ESTRUCTURA AUXILIAR
 * *****************************************************************/

//Estructura auxiliar con lo que se necesita para agregar/borrarle un hijo al abb.
typedef struct{

	bool izq;
	nodo_t* padre;
}padre_t;
/* ******************************************************************
 *                   FUNCIONES AUXILIARES
 * *****************************************************************/
padre_t* padre_crear(void){
	padre_t* padre = malloc(sizeof(padre_t));
	if(!padre)
		return NULL;
	padre->padre = NULL;
	padre->izq = true;
	return padre;
}

//Recorre el árbol buscando un nodo con la clave pasada por parámetro.
//Además, se pasa un doble puntero para obtener al padre del nodo con la clave.
//Si no se encuentra al nodo, devuelve NULL.
 nodo_t* abb_recorrer(nodo_t* nodo, const char* clave, abb_comparar_clave_t cmp, padre_t* extra){
 	if(!nodo)
 		return NULL;

 	//Si encuentra la clave en el árbol, devuelve el nodo que la contiene.
 	int comparacion_clave = cmp(nodo->clave, clave);
 	if(!comparacion_clave)
 		return nodo;

 	//Si no, busca a izquierda o derecha según la clave sea mayor o menor al nodo actual.
 	if(extra)
 		extra->padre = nodo;
 	if(comparacion_clave < 0){
 		if(extra)
 			extra->izq = false;
 		return abb_recorrer(nodo->der, clave, cmp, extra);
 	}

   else{
   		if(extra)
   			extra->izq = true;
 		return abb_recorrer(nodo->izq, clave, cmp, extra);
   }

 }

nodo_t* traza_izquierda(nodo_t* nodo){

	nodo_t* minimo = nodo;
	while(minimo->izq)
		minimo = minimo->izq;
	
	return minimo;
}

/* ******************************************************************
 *                   	WRAPPERS
 * *****************************************************************/

void _abb_destruir(nodo_t* nodo, abb_destruir_dato_t destruir_dato){

	if(!nodo)
		return;

	_abb_destruir(nodo->izq, destruir_dato);
	_abb_destruir(nodo->der, destruir_dato);

	lista_destruir(nodo->lista, destruir_dato);
	free(nodo->clave);
	free(nodo);
	return;
}

void apilar_hijos_izquierdos(pila_t* pila, nodo_t* nodo){

	nodo_t* actual = nodo;
	while(actual){
		pila_apilar(pila, actual);
		actual = actual->izq;
	}
}

bool abb_iterar(nodo_t* nodo, bool visitar(const char*, lista_t*, void*), void* extra){

	if(!nodo)
		return true;

	if(!abb_iterar(nodo->izq, visitar, extra))
		return false;

	if(!visitar(nodo->clave, nodo->lista, extra))
		return false;
	return abb_iterar(nodo->der, visitar, extra);
}


/* ******************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){

	abb_t* abb = malloc(sizeof(abb_t));
	if(!abb)
		return NULL;
	abb->destruir_dato = destruir_dato;
	abb->comparar_clave = cmp;
	abb->cantidad = 0;
	abb->raiz = NULL;
  return abb;
}

bool abb_guardar(abb_t *abb, const char *clave, void *dato){

	padre_t* extra = padre_crear();
	if(!extra)
		return false;

 	nodo_t* nodo_misma_clave = abb_recorrer(abb->raiz, clave, abb->comparar_clave, extra);
 	if(!nodo_misma_clave){
 		nodo_t* a_guardar = nodo_crear(clave, dato);
 		if(!a_guardar)
 			return false;
	 	if(!extra->padre)
	 		abb->raiz = a_guardar;
	 	else{
	 		if(!extra->izq)
	 			extra->padre->der = a_guardar;
	 		else
	 			extra->padre->izq = a_guardar;
	 	}
	 	abb->cantidad++;
	 }
	else
	 	lista_insertar_ultimo(nodo_misma_clave->lista, dato);
	 	
	free(extra);
	return true;
}

lista_t* abb_borrar(abb_t *arbol, const char *clave){

	padre_t* a_borrar_padre = padre_crear();
	if(!a_borrar_padre)
		return NULL;

	nodo_t* a_borrar = abb_recorrer(arbol->raiz, clave, arbol->comparar_clave, a_borrar_padre);
	if(!a_borrar){
		free(a_borrar_padre);
		return NULL;
	}
	
	lista_t* a_devolver = a_borrar->lista;

	//Primer caso: a_borrar es un nodo interno con dos hijos.
	if(a_borrar->izq && a_borrar->der){
	
		nodo_t* reemplazante_nodo = traza_izquierda(a_borrar->der);
		char* reemplazante_clave = strdup(reemplazante_nodo->clave);
		lista_t* reemplazante_lista = abb_borrar(arbol, reemplazante_nodo->clave);
		free(a_borrar->clave);
		a_borrar->clave = reemplazante_clave;
		a_borrar->lista = reemplazante_lista;
	}
	else{

		free(a_borrar->clave);
		
		//Segundo caso: a_borrar es una hoja (nodo sin hijos)
		if(!a_borrar->izq && !a_borrar->der){
			free(a_borrar);

			//Caso raíz

			if(!a_borrar_padre->padre)
				arbol->raiz = NULL;

			//Caso general

			else if(a_borrar_padre->izq)
				a_borrar_padre->padre->izq = NULL;
			else
				a_borrar_padre->padre->der = NULL;
		}
		//Tercer caso: a_borrar es un nodo interno con un hijo.
		else{

			nodo_t* hijo = (!a_borrar->izq && a_borrar->der) ? a_borrar->der : a_borrar->izq;

			//Caso raíz

			if(!a_borrar_padre->padre)
				arbol->raiz = hijo;

			//Caso general
			else{
				if(a_borrar_padre->izq)
					a_borrar_padre->padre->izq = hijo;
				else
					a_borrar_padre->padre->der = hijo;
			}
			free(a_borrar);
		
		}
		arbol->cantidad--;
	}

	free(a_borrar_padre);
	return a_devolver;
}




lista_t* abb_obtener(const abb_t *abb, const char *clave){

	nodo_t* nodo = abb_recorrer(abb->raiz, clave, abb->comparar_clave, NULL);
	if(!nodo)
		return NULL;

	return nodo->lista;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){

	return (abb_recorrer(arbol->raiz, clave, arbol->comparar_clave, NULL)) ? true : false;
}

size_t abb_cantidad(abb_t *abb){
	return abb->cantidad;

}

void abb_destruir(abb_t *arbol){
	_abb_destruir(arbol->raiz, arbol->destruir_dato);
	free(arbol);
}

/* ******************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/


void abb_in_order(abb_t* arbol, bool visitar(const char*, lista_t*, void*), void* extra){
	if(!arbol->raiz)
		return;
	abb_iterar(arbol->raiz, visitar, extra);
}

/* *****************************************************************
 *              PRIMITIVAS DEL ITERADOR EXTERNO 
 * *****************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t* arbol){

	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if(!iter)
		return NULL;

	iter->comparar_clave = arbol->comparar_clave;
	iter->pila = pila_crear();
	if(arbol->raiz)
		apilar_hijos_izquierdos(iter->pila, arbol->raiz);
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){

	if(pila_esta_vacia(iter->pila))
		return false;

	nodo_t* padre = pila_desapilar(iter->pila);
	if(padre->der)
		apilar_hijos_izquierdos(iter->pila, padre->der);

	return true;
}

const char* abb_iter_in_ver_actual(const abb_iter_t* iter){

	if(pila_esta_vacia(iter->pila))
		return NULL;

	nodo_t* actual = pila_ver_tope(iter->pila);
	return actual->clave;

}

bool abb_iter_in_al_final(const abb_iter_t* iter){

	return pila_esta_vacia(iter->pila);
}

const char* abb_iter_in_buscar_clave(abb_iter_t* iter, const char* desde){

	while(!abb_iter_in_al_final(iter) && iter->comparar_clave(abb_iter_in_ver_actual(iter), desde) <= 0)
		abb_iter_in_avanzar(iter);
	
	return (abb_iter_in_al_final(iter)) ? NULL : abb_iter_in_ver_actual(iter);
}

void abb_iter_in_destruir(abb_iter_t* iter){
	pila_destruir(iter->pila);
	free(iter);
}
