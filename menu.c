/*
 ============================================================================
 Nome         : menu.c
 Autores      : Jéssika Darambaris e Roberto Freitas
 Descrição    : Arquivo responsável por tratar a entrada e exibir o menu
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "CompressaoSemPerdas.c"
#include "CompressaoComPerdas.c"
#include "DescompressaoSemPerdas.c"
#include "DescompressaoComPerdas.c"

#define USAGE "\
Exemplo de Uso: make nome-da-imagem.bmp \n\
Obs: Colocar imagem BMP na pasta imagens... \n\n"


/* assinaturas */
int parametrosEntrada(int, char**);
int iniciaMenu(char**);

int main (int argc, char *argv[]){	
	
	if (parametrosEntrada(argc, argv)) {
		return EXIT_FAILURE;
	}

	/* inicia o menu */
	if (iniciaMenu(argv)) {
		return EXIT_FAILURE;
	}	

	return 0;
}

/* trata os parâmetros de entrada, verificando se há alguma imagem .bmp */
int parametrosEntrada(int argc, char** argv){
	
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

int iniciaMenu(char** argv){
	char opcao;

	printf("\n ------------------ Compressor ------------------------- \n\n");
	printf("1 - Compressão Sem Perdas \n");
	printf("2 - Compressão Com Perdas \n");
	printf("3 - Compressão Sem Perdas e Compressão Com Perdas \n");
	printf("4 - Compressão Com Perdas e Compressão Sem Perdas \n");
	printf("\n\nDigite a opção desejada (1-4): ");
	scanf("%c",&opcao);
	getchar();
	
	switch(opcao){
		case '1':	{
			if (compressaoSemPerdas())
				return EXIT_FAILURE;			
			break;
		}	 
		case '2':	{	
			if (compressaoComPerdas())
				return EXIT_FAILURE;	
			break;
		}	
		case '3':	{
			if (compressaoSemPerdas())
				return EXIT_FAILURE;			
			break;
		}	
		case '4':	{
			if (compressaoSemPerdas())
				return EXIT_FAILURE;		
			break;
		}	
	}

	printf("\n ------------------ Descompressor ------------------------- \n\n");
	printf("1 - Descompressão Sem Perdas \n");
	printf("2 - Descompressão Com Perdas \n");
	printf("3 - Sem Descompressão \n");
	printf("\n\nDigite a opção desejada (1-3): ");
	
	scanf("%c",&opcao);
	getchar();
	
	switch(opcao){
		case '1':	{
			if (descompressaoSemPerdas())
				return EXIT_FAILURE;			
			break;
		}	 
		case '2':	{
			if (descompressaoComPerdas())
				return EXIT_FAILURE;		
			break;
		}	
		case '3':	{
			return EXIT_SUCCESS;		
			break;
		}	
	}	
}

	