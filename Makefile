############################# Makefile ##########################
CC=gcc
CFLAGS=-lm -std=c99 
OBJ=arquivo.o main.o dct.o comPerdas.o
HEADERS= arquivo.h dct.h comPerdas.h
all: projeto
projeto: $(OBJ) 
	$(CC) -o projeto $(OBJ) $(CFLAGS)
arquivo.o: arquivo.c
	$(CC) -o arquivo.o -c arquivo.c $(CFLAGS)
comPerdas.o: comPerdas.c
	$(CC) -o comPerdas.o -c comPerdas.c $(CFLAGS)
dct.o: dct.c
	$(CC) -o dct.o -c dct.c $(CFLAGS)
main.o: main.c $(HEADERS)
	$(CC) -o main.o -c main.c $(CFLAGS)
clean:
	rm -rf *.o
	rm -rf projeto
