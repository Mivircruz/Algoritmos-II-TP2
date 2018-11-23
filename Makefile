CC = gcc
CFLAGS = -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion 
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

CFILES = main.c comandos.c strutil.c procesamiento.c abb.c hash.c heap.c pila.c

HFILES = comandos.h strutil.h procesamiento.h abb.h hash.h heap.h pila.h
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
