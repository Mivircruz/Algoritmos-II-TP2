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
	comando = NULL;
	long int vuelo = 0;
	char* linea = NULL;
	size_t capacidad = 0;
	hash_t* hash = hash_crear(NULL);
	abb_t* abb = abb_crear(strcmp, NULL);

//Procesamiento de comandos

	while(getline(&linea, &capacidad, stdin) != -1){

		if(!validar_argumentos(split(linea, ' '), &comando, &vuelo)){
			if(!comando)
				fprintf(stderr, "Error\n");
			else{
				free(comando);
				fprintf(stderr, "Error en comando %s\n", comando);
			}
		}
		else
			printf("NO ERROR :)\n");
	}
	hash_destruir(hash);
	abb_destruir(abb);
	return 0;
}