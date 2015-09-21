#include <stdlib.h>
#include <stdio.h>
#include "arquivo.h"


char *leArquivo(char path[], int *tam){

	FILE *arq = fopen(path,"rb");
	
	
	if (arq == NULL){
		printf("\nHouve um erro ao abrir o arquivo. \n");
		exit(1);
	}

	fseek(arq, 0L, SEEK_END);
	*tam = ftell(arq);
	fseek(arq,0,SEEK_SET);

	 char *dados = ( char*)malloc(sizeof( char)*(*tam));
	
	if (dados == NULL){
		printf("\nErro de Memória \n");
		exit(1);
	}	

	 int verificaLeitura = fread(dados, 1, *tam, arq);

	if(verificaLeitura != *tam){
        printf("\nErro na Leitura do Arquivo\n");
        exit(1);
    }

   fclose(arq);
   return dados;
}

int *leArquivoEmBlocos(char path[], HEADERBMP *header, INFOHEADERBMP *infoHeader, LISTABLOCOS *bloco){
	return 0;
}