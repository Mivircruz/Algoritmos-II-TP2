CC = gcc
CFLAGS = -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion 
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

CFILES = main.c comandos.c strutil.c procesamiento.c vuelos.c abb.c cola.c hash.c heap.c pila.c

HFILES = deps.mk comandos.h strutil.h procesamiento.h vuelos.h abb.h cola.h hash.h heap.h pila.h
EXEC = pruebas

build: $(CFILES) 
	$(CC) $(CFLAGS)  -o $(EXEC) $(CFILES) 

run: build
	 ./$(EXEC) 

valgrind: build
	valgrind $(VFLAGS) ./$(EXEC) 
	
gdb: build
	gdb $(GDBFLAGS)./$(EXEC)

zip: build
	zip $(EXEC).zip $(CFILES) $(HFILES)

clean:
	rm -f *.o $(EXEC)
