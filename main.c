 #define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "comandos.h"
#include "strutil.h"
#include "hash.h"
#include "abb.h"

int main(int argc, char* argv[]){

//Inicialización de variables 

	char* comando;
	char* linea = NULL;
	size_t capacidad = 0;
	hash_t* hash = hash_crear(NULL);
	abb_t* abb = abb_crear(strcmp, NULL);
	char** vector_linea;

//Procesamiento de comandos

	while(getline(&linea, &capacidad, stdin) != -1){
		
		comando = NULL;
		vector_linea = split(linea, ' ');
		if(!ejecutar_comandos(vector_linea, hash, abb, &comando)){
			if(!comando)
				fprintf(stderr, "Error\n");
			else{
				fprintf(stderr, "Error en comando %s\n", comando);
				free(comando);
			}
		}
		else
			printf("OK\n");
		
		if(!hash_cantidad(hash))
			break;
	}
	hash_destruir(hash);
	abb_destruir(abb);
	return 0;
}
