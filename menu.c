/*
 ============================================================================
 Name        : menu.c
 Author      : Jéssika Darambaris e Roberto Freitas
 Description : Arquivo responsável por tratar a entrada e exibir o menu
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define USAGE "\
Exemplo de Uso: make nome-da-imagem.bmp \n\
Obs: Colocar imagem BMP na pasta imagens... \n\n"

/* trata os parâmetros de entrada, verificando se há alguma imagem .bmp */
int parametrosEntrada(int argc, char** argv, char** option){
	
	char *img = malloc(sizeof(argv[1]));	
	strcpy(img,argv[1]);

	if (argc == 1){
		printf(USAGE);
		return EXIT_FAILURE;
	}

	char *ext;

	ext = strtok(img,".");
	ext = strtok(NULL,".");

	/* verifica se a extensão é do tipo bitmap */
	if (strcmp(ext,"BMP") && strcmp(ext,"bmp")) {
		printf("A imagem possui uma extensão .%s .Por favor, entre com uma imagem do tipo bitmap \n",ext);
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

int main (int argc, char *argv[]){	
	char *option = NULL;
	
	if (parametrosEntrada(argc, argv, &option)) {
		return EXIT_FAILURE;
	}

	/* inicia o menu */

	return 0;
}