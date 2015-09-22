/*
 ============================================================================
 Nome         : main.c
 Autores      : Jéssika Darambaris e Roberto Freitas
 Descrição    : Arquivo responsável por tratar a entrada e exibir o menu
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "arquivo.h"
#include "comPerdas.h"

#include "compressaosemperdas.c"
//#include "CompressaoComPerdas.c"
//#include "DescompressaoSemPerdas.c"

#define USAGE "\
Exemplo de Uso: ./trabalho nome-da-imagem.bmp \n\
Obs: Colocar imagem BMP na pasta imagens... \n\n"


/* assinaturas */
int trataParametrosEntrada(int, char**);
int iniciaMenu(char**, char[]);

int main (int argc, char *argv[]){	
	
	if (trataParametrosEntrada(argc, argv)) {
		return EXIT_FAILURE;
	}
	
	/* monta o caminho da imagem (pasta imagens) */
	char **path = malloc(sizeof(char *)*4);
	path[0] = malloc (sizeof(argv[1])+15);

	strcat(path[0],"imagens/");
	strcat(path[0],argv[1]);

	/* inicia o menu */
	if (iniciaMenu(argv,path[0])) {
		return EXIT_FAILURE;
	}

	return 0;
}

/* trata os parâmetros de entrada, verificando se há alguma imagem .bmp */
int trataParametrosEntrada(int argc, char** argv){
	
	if (argc == 1){
		printf(USAGE);
		return EXIT_FAILURE;
	}

	char *ext;
	char *img = malloc(sizeof(argv[1]));	
	strcpy(img,argv[1]);

	ext = strtok(img,".");
	ext = strtok(NULL,".");

	/* verifica se a extensão é do tipo bitmap */
	if (strcmp(ext,"BMP") && strcmp(ext,"bmp")) {
		printf("A imagem possui uma extensão .%s .Por favor, entre com uma imagem do tipo bitmap \n",ext);
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

int iniciaMenu(char** argv, char path[]){
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
			unsigned int tamanho;
			unsigned char *dados;
			
			/* lê os bytes do arquivo de imagem e guarda em dados */
			dados = (char *) leArquivo(path,&tamanho);			
			
			/* chama a função que executa o código de huffman (compressão sem perdas) */
			compressaoSemPerdas((char *)dados,tamanho);

			break;
		}	 
		case '2':	{	
			
			// Headers do BMP original
    		HeaderBMP header;
    		InfoHeaderBMP infoHeader;
    		
    		// vetor de blocos RGB
    		BlocoRGB *vetorBlocos;

    		//
    		int qtdeBlocos;
    		leArquivoEmBlocos(path, &header, &infoHeader,&vetorBlocos,&qtdeBlocos);
			//compressaoComPerdas(&vetorBlocos,qtdeBlocos);
			//descompressaoComPerdas(&vetorBlocos,qtdeBlocos);
			//printvetorBlocos(vetorBlocos,qtdeBlocos);
			//gravaArquivoEmBlocos(path,header,infoHeader,vetorBlocos);
			break;
		}	
	}
}
	
