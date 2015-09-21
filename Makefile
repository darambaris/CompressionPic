############################# Makefile ##########################
CC=gcc
CFLAGS=-std=c99 -lm 
OBJ=arquivo.o main.o
HEADERS= arquivo.h
all: projeto
projeto: $(OBJ) 
	$(CC) -o projeto $(OBJ)
arquivo.o: arquivo.c
	$(CC) -o arquivo.o -c arquivo.c $(CFLAGS)
main.o: main.c $(HEADERS)
	$(CC) -o main.o -c main.c $(CFLAGS)
clean:
	rm -rf *.o
	rm -rf projeto
