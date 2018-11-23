#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "heap.h"

#define TAM_INICIAL 		30
#define CAPACIDAD_MINIMA	13
#define FACTOR_REDIMENSION 	2

/* *****************************************************************
 *                   DEFINICIÓN DE LA ESTRUCTURA
 * *****************************************************************/

struct heap{

	void** vector;
	cmp_func_t funcion_comparar;
	size_t cantidad;
	size_t capacidad;
};

/* *****************************************************************
 *                   	FUNCIONES AUXILIARES
 * *****************************************************************/

void swap(void** a, void** b){

	void* aux = *b;
	*b = *a;
	*a = aux;
}


void downheap(void* arreglo[], size_t pos_padre, size_t pos_hijo_izq, size_t pos_hijo_der, cmp_func_t cmp, size_t tam){

	if(pos_hijo_izq >= tam)
		return;

	size_t pos_hijo_a_cambiar;

	if(pos_hijo_der < tam){
		if(cmp(arreglo[pos_padre], arreglo[pos_hijo_izq]) < 0 ||  cmp(arreglo[pos_padre], arreglo[pos_hijo_der]) < 0)
			pos_hijo_a_cambiar = (cmp(arreglo[pos_hijo_izq], arreglo[pos_hijo_der]) < 0) ? pos_hijo_der : pos_hijo_izq;
		else return;
	}
	else{
		pos_hijo_a_cambiar =  pos_hijo_izq;
		if(cmp(arreglo[pos_padre], arreglo[pos_hijo_a_cambiar]) > 0)
			return;
	}
	swap(&(arreglo[pos_padre]), &(arreglo[pos_hijo_a_cambiar]));
	pos_padre = pos_hijo_a_cambiar;
	
	downheap(arreglo, pos_padre, pos_padre*2 + 1, pos_padre*2 + 2, cmp, tam);

}

void upheap(void* arreglo[], size_t pos_hijo, cmp_func_t cmp){

	size_t pos_padre = (pos_hijo-1)/2;
	while(cmp(arreglo[pos_padre], arreglo[pos_hijo]) < 0){
		swap(&(arreglo[pos_padre]), &(arreglo[pos_hijo]));
		if(!pos_padre)
			break;

		pos_hijo = pos_padre;
		pos_padre = (pos_padre-1)/2;
	}
}

void heapify(void* arreglo[], size_t n, cmp_func_t cmp){

	for(long int i = (long int)(n/2 - 1); 0 <= i; i--)
		downheap(arreglo, i, i*2+1, i*2+2, cmp, n);
}

bool heap_a_redimensionar(heap_t* heap, size_t tam){
	if(heap->capacidad > tam){
		if(heap->capacidad <= CAPACIDAD_MINIMA)
			return true;
	}
	void** aux = realloc(heap->vector, sizeof(void*) * tam);
	if(!aux)
		return false;
	heap->vector = aux;
	heap->capacidad = tam;
	return true;
}

/* *****************************************************************
 *                    PRIMITIVAS DEL HEAP
 * *****************************************************************/


heap_t *heap_crear(cmp_func_t cmp){

	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap)
		return NULL;
	heap->vector = malloc(sizeof(void*) * TAM_INICIAL);
	if(!heap->vector){
		free(heap);
		return NULL;
	}
	heap->cantidad = 0;
	heap->capacidad = TAM_INICIAL;
	heap->funcion_comparar = cmp;
	return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){

	heap_t* heap = heap_crear(cmp);
	if(!heap)
		return NULL;

	void** vector = malloc(sizeof(void*) * n);
	for(int i = 0; i < n; i++)
		vector[i] = arreglo[i];

	heapify(vector, n, cmp);
	free(heap->vector);
	heap->vector = vector;
	heap->cantidad = n;
	heap->capacidad = n;
	heap->funcion_comparar = cmp;
	return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){

	if(destruir_elemento){
		for(size_t i = 0; i < heap->cantidad; i++)
			destruir_elemento(heap->vector[i]);
	}
	free(heap->vector);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap){
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
	return (heap->cantidad) ? false : true;
}



void *heap_ver_max(const heap_t *heap){
	if(heap_esta_vacio(heap))
		return NULL;

	return *(heap->vector);
}

bool heap_encolar(heap_t *heap, void *elem){

	if(heap->cantidad == heap->capacidad){
		if(!heap_a_redimensionar(heap, heap->capacidad * FACTOR_REDIMENSION))
			return false;
	}
	heap->vector[heap->cantidad] = elem;
	if(heap->cantidad)
		upheap(heap->vector, heap->cantidad, heap->funcion_comparar);
	heap->cantidad++;
	return true;

}

void *heap_desencolar(heap_t *heap){

	if(heap_esta_vacio(heap))
		return NULL;

	if(heap->cantidad <= (heap->capacidad)/4)
		heap_a_redimensionar(heap, heap->capacidad/FACTOR_REDIMENSION);
	void* a_devolver = heap->vector[0];
	swap(&(heap->vector[0]), &(heap->vector[heap->cantidad-1]));
	heap->cantidad--;
	downheap(heap->vector, 0, 1, 2,  heap->funcion_comparar, heap->cantidad);
	return a_devolver;
}


void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heapify(elementos, cant, cmp);
	size_t ultimo_relativo = cant-1;
	for(size_t i = 0; i < cant; i++){
		swap(&(elementos[0]), &(elementos[ultimo_relativo]));
		downheap(elementos, 0, 1, 2, cmp, ultimo_relativo);
		ultimo_relativo--;
	}
}