 #define _GNU_SOURCE


#include <stdio.h>
#include <stdbool.h>
#include "comandos.h"
#include "strutil.h"

int main(int argc, char* argv[]){

//Inicialización de variables 

	comando_t* comando = NULL;
	long int* vuelo = 0;
	char* linea = NULL;
	size_t capacidad = 0;

//Procesamiento de comandos

	while(getline(&linea, &capacidad, stdin) != -1){
	if(!validar_argumentos(split(linea, ' '), comando, vuelo)){
		printf("ERROR :)\n");
	}
	else
		printf("LSEGIOEWGOH NO\n");
	}
	return 0;
}