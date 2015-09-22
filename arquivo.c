#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arquivo.h"
#include "comPerdas.h"
#include "dct.h"

unsigned char *leArquivo(unsigned char path[], unsigned int *tam){

	FILE *arq = fopen(path,"rb");
	
	
	if (arq == NULL){
		printf("\nHouve um erro ao abrir o arquivo. \n");
		exit(1);
	}

	fseek(arq, 0L, SEEK_END);
	*tam = ftell(arq);
	fseek(arq,0,SEEK_SET);

	unsigned char *dados = ( char*)malloc(sizeof( char)*(*tam));
	
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

int *leArquivoEmBlocos(char path[], HeaderBMP *header, InfoHeaderBMP *infoHeader, BlocoRGB **vetBlocosMain, int *qtdeBlocos){
	/* numero mágico do cabeçalho bmp */
	NumMagicoBMP numMagic; 
	int numBlocosLargura, numBlocosAltura;
	int restoPixelsLargura, restoPixelsAltura;
	/* abre o arquivo em modo leitura para binário */
	FILE *arq = fopen(path,"rb");

	if (arq == NULL){
		printf("\nHouve um erro ao abrir o arquivo. \n");
		exit(1);
	}

	/* lê o número mágico que possui 2 bytes */
	fread(&numMagic, 2, 1, arq);
	
	/* lê o cabeçalho do bmp */
	fread(header, sizeof(HeaderBMP),1, arq);

	/* lẽ as informações contidas sobre o cabeçalho bmp */
	fread(infoHeader, sizeof(InfoHeaderBMP), 1, arq);

	//verifica se o arquivo é um bitmap VER SE É NECESSÁRIO POR DEPOIS */

	/* calcula o número de blocos 8x8 que possui na largura e na altura */
	numBlocosLargura = infoHeader->biWidth / 8; 
	numBlocosAltura = infoHeader->biHeight / 8;

	/* calcula o resto que sobrou de pixels na largura e na altura */
	restoPixelsLargura = infoHeader->biWidth % 8;
	restoPixelsAltura = infoHeader->biHeight % 8;

	
	/* calcula tamanho inicial do vetor de Blocos */ 
	BlocoRGB *vetorBlocos = (BlocoRGB *) malloc(sizeof(BlocoRGB)*((numBlocosLargura+1)*(numBlocosAltura+1)));
	*vetBlocosMain = (BlocoRGB *) malloc(sizeof(BlocoRGB)*((numBlocosLargura+1)*(numBlocosAltura+1)));

	if(restoPixelsLargura) printf("TEM SOBRA!!!");

	int col,lin,k,i,j,qtde_blocos = 0,qtde_colunas=8;
	
	for (col=0;col<=numBlocosAltura;col++){
		
		if (col==numBlocosAltura){
			if (!restoPixelsAltura) break;
			qtde_colunas = restoPixelsAltura;
		}

		for (k=0;k<qtde_colunas;k++){	
			/* pega os valores RGB de uma linha até um número múltiplo de 8 */
			for (lin=0;lin<numBlocosLargura;lin++){
				
				/* separa em blocos de 8 pixels */
				for (i=0;i<8;i++){
					vetorBlocos[lin+qtde_blocos].r[k][i] = fgetc(arq);
					vetorBlocos[lin+qtde_blocos].g[k][i] = fgetc(arq);
					vetorBlocos[lin+qtde_blocos].b[k][i] = fgetc(arq);	
				}

			}

			/* verifica se o último bloco da largura possui pixel sobrando */
			if (restoPixelsLargura)	{
				for (i=0;i<restoPixelsLargura;i++){
					vetorBlocos[numBlocosLargura+qtde_blocos].r[k][i] = fgetc(arq);
					vetorBlocos[numBlocosLargura+qtde_blocos].g[k][i] = fgetc(arq);
					vetorBlocos[numBlocosLargura+qtde_blocos].b[k][i] = fgetc(arq);
				}
		
				for (i;i<8;i++){
					vetorBlocos[numBlocosLargura+qtde_blocos].r[k][i] = 0;
					vetorBlocos[numBlocosLargura+qtde_blocos].g[k][i] = 0;
					vetorBlocos[numBlocosLargura+qtde_blocos].b[k][i] = 0;	
				}

			}
				
		}	

		qtde_blocos += (restoPixelsLargura)?numBlocosLargura+1:numBlocosLargura;
	}

	//if(!restoPixelsAltura) qtde_blocos -= (restoPixelsLargura)?numBlocosLargura+1:numBlocosLargura;
	

	if(restoPixelsAltura){
		for (k=restoPixelsAltura;k<8;k++){
			/* pega os valores RGB de uma linha até um número múltiplo de 8 */
				for (lin=0;lin<numBlocosLargura;lin++){
					/* separa em blocos de 8 pixels */
					for (i=0;i<8;i++){
						vetorBlocos[qtde_blocos-lin].r[k][i] = 0;
						vetorBlocos[qtde_blocos-lin].g[k][i] = 0;
						vetorBlocos[qtde_blocos-lin].b[k][i] = 0;	
					}

				}
		}
	}

	*qtdeBlocos = qtde_blocos;
	/*copia vetor para main */
	/*for (i=0;i<qtde_blocos;i++){
		for (k=0;k<8;k++){
			for (j=0;j<8;j++){
				(*vetBlocosMain)[i].r[j][k] = vetorBlocos[i].r[j][k];
				(*vetBlocosMain)[i].r[j][k] = vetorBlocos[i].g[j][k];
				(*vetBlocosMain)[i].r[j][k] = vetorBlocos[i].b[j][k];
			}
		}
	}*/
	
	compressaoComPerdas(&vetorBlocos,*qtdeBlocos);
	descompressaoComPerdas(&vetorBlocos,*qtdeBlocos);
	
	
	gravaArquivoEmBlocos(path,*header,*infoHeader,vetorBlocos);

	free(vetorBlocos);
	fclose(arq);
}

int gravaArquivoEmBlocos(char path[],HeaderBMP header, InfoHeaderBMP infoHeader,BlocoRGB vetorBlocos[]){
	
	path = strtok(path,"/");
	path = strtok(NULL,"/");

	char *nomeArquivo = malloc (sizeof(path)+30);
	strcat(nomeArquivo, "imagens/");
	strcat(nomeArquivo, "com-perdas-");
	strcat(nomeArquivo,path);

	FILE *arq = fopen(nomeArquivo,"wb+");
	unsigned short numMagic = 19778;

	// salva o header de arquivo (fheader) com 14 bytes
    fwrite(&numMagic, 2, 1, arq);
    fwrite(&header, sizeof(HeaderBMP), 1, arq);
    
    // salva o header de informacoes (iheader).
    fwrite(&infoHeader, sizeof(InfoHeaderBMP), 1, arq);
	
	int col,k,lin,i,qtde_blocos=0,qtde_colunas=8; 

	int numBlocosAltura = infoHeader.biHeight / 8;
	int numBlocosLargura = infoHeader.biWidth / 8;

	int restoPixelsAltura = infoHeader.biHeight % 8;
	int restoPixelsLargura = infoHeader.biWidth % 8;

	for (col=0;col<=numBlocosAltura;col++){

		if (col==numBlocosAltura){
			if (!restoPixelsAltura) break;
			qtde_colunas = restoPixelsAltura;
		}

		for (k=0;k<qtde_colunas;k++){	
			/* pega os valores RGB de uma linha até um número múltiplo de 8 */
			for (lin=0;lin<numBlocosLargura;lin++){
				/* separa em blocos de 8 pixels */
				for (i=0;i<8;i++){
					 fputc(vetorBlocos[lin+qtde_blocos].r[k][i],arq);
					 fputc(vetorBlocos[lin+qtde_blocos].g[k][i],arq);
					 fputc(vetorBlocos[lin+qtde_blocos].b[k][i],arq);	
				}
			}

			for (i=0; i<restoPixelsLargura; i++) {
				fputc(vetorBlocos[numBlocosLargura+qtde_blocos].r[k][i],arq);
				fputc(vetorBlocos[numBlocosLargura+qtde_blocos].g[k][i],arq);
				fputc(vetorBlocos[numBlocosLargura+qtde_blocos].b[k][i],arq);	
			}

		}
		qtde_blocos += (restoPixelsLargura)?numBlocosLargura+1:numBlocosLargura;
	}

	fclose(arq);
}

//debug
void printvetorBlocos(BlocoRGB vetorBlocos[], int numBlocos){
	int i,j,k;
	for (i=0;i<numBlocos;i++){
		for (k=0;k<8;k++){
			for (j=0;j<8;j++)
				printf("%d ", vetorBlocos[i].r[j][k]);
			printf("\n");
		}

		printf(" Bloco: %d\n",i);
	}
}