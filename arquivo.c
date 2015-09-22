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

int leArquivoComprimidoDCT(char path[], HeaderBMP *header, InfoHeaderBMP *infoHeader){
	
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


	char *vetorR = (char *) malloc(sizeof(char)*64);
	char *vetorG = (char *) malloc(sizeof(char)*64);
	char *vetorB = (char *) malloc(sizeof(char)*64);
	
	int zerosR, zerosG, zerosB,qtde_blocos=0,i;


	BlocoRGB *vetorBlocos = (BlocoRGB *) malloc (sizeof(BlocoRGB));
	
	while (1){
			
		zerosR = fgetc(arq);

		if (zerosR == EOF) break; 

		qtde_blocos+=1;


		vetorBlocos = realloc (vetorBlocos,sizeof(BlocoRGB)*(qtde_blocos));
		for (i=0;i<=(63-zerosR);i++) vetorR[i]= fgetc(arq);
		for (i;i<64;i++) vetorR[i] = 0;

		zerosG = fgetc(arq);
		
		for (i=0;i<=(63-zerosG);i++) vetorG[i]= fgetc(arq);
		for (i;i<64;i++) vetorG[i] = 0;
		
		zerosB = fgetc(arq);
		
		for (i=0;i<=(63-zerosB);i++) vetorB[i]= fgetc(arq);
		for (i;i<64;i++) vetorB[i] = 0;	

		desvetorizaBlocoZigueZague(vetorBlocos[qtde_blocos-1].r,&vetorR);
		desvetorizaBlocoZigueZague(vetorBlocos[qtde_blocos-1].g,&vetorG);
		desvetorizaBlocoZigueZague(vetorBlocos[qtde_blocos-1].b,&vetorB);	
	}	
	printf("%d\n",qtde_blocos);	
	//compressaoComPerdas(&vetorBlocos,qtde_blocos);
	descompressaoComPerdas(&vetorBlocos,qtde_blocos);
	
	//printvetorBlocos(vetorBlocos,qtde_blocos);
	gravaArquivoEmBlocos(path,*header,*infoHeader,vetorBlocos);

	free(vetorR);
	free(vetorG);
	free(vetorB);
	free(vetorBlocos);

	fclose(arq);
	

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
	//*vetBlocosMain = (BlocoRGB *) malloc(sizeof(BlocoRGB)*((numBlocosLargura)*(numBlocosAltura)));



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

	printf("%d\n",qtde_blocos);	


	compressaoComPerdas(&vetorBlocos,qtde_blocos);
	//descompressaoComPerdas(&vetorBlocos,qtde_blocos);
	
	gravaArquivoComprimidoDCT(path,*header,*infoHeader,vetorBlocos, qtde_blocos);
	//gravaArquivoEmBlocos(path,*header,*infoHeader,vetorBlocos);
	//printvetorBlocos(vetorBlocos,qtde_blocos-lin);
	free(vetorBlocos);
	fclose(arq);
}

int gravaArquivoComprimidoDCT(char path[], HeaderBMP header, InfoHeaderBMP infoHeader, BlocoRGB vetorBlocos[], int qtdeBlocos){
	/*path = strtok(path,"/");
	path = strtok(NULL,"/");
	
	char *nomeArquivo = malloc (sizeof(path)+30);
	strcat(nomeArquivo, "imagens/");
	strcat(nomeArquivo, "com-perdas-");
	path = strtok(NULL,".");
	strcat(nomeArquivo,path);
	strcat(nomeArquivo,".dat");*/

	FILE *arq = fopen("imagens/compressao.dat","wb+");
	
	if(arq==NULL) printf("Erro abrir arquivo");

	unsigned short numMagic = 19778;

	// salva o header de arquivo (fheader) com 14 bytes
    fwrite(&numMagic, 2, 1, arq);
    fwrite(&header, sizeof(HeaderBMP), 1, arq);
    
    // salva o header de informacoes (iheader).
    fwrite(&infoHeader, sizeof(InfoHeaderBMP), 1, arq);

    int zerosR,zerosG,zerosB,posR,posG,posB,i,j;
  
  	char *vetorR = (char *) malloc (sizeof(char)*64);
  	char *vetorG = (char *) malloc (sizeof(char)*64);
  	char *vetorB = (char *) malloc (sizeof(char)*64);

	for (i=0;i<qtdeBlocos;i++){
		vetorizaBlocoZigueZague(vetorBlocos[i].r,&vetorR);
		vetorizaBlocoZigueZague(vetorBlocos[i].g,&vetorG);
		vetorizaBlocoZigueZague(vetorBlocos[i].b,&vetorB);
		
		zerosR = contagemDeZeros(vetorR,&posR);
		zerosG = contagemDeZeros(vetorG,&posG);
		zerosB = contagemDeZeros(vetorB,&posB);
		
		fputc(zerosR,arq);	
		for (j=0;j<=posR;j++) fputc(vetorR[j],arq);
		
		fputc(zerosG,arq);
		for (j=0;j<=posG;j++) fputc(vetorG[j],arq);
		
		fputc(zerosB,arq);
		for (j=0;j<=posB;j++) fputc(vetorB[j],arq);
		
	}

	free(vetorR);
	free(vetorG);
	free(vetorB);
    //free(nomeArquivo);
    fclose(arq);
} 

int gravaArquivoEmBlocos(char path[],HeaderBMP header, InfoHeaderBMP infoHeader,BlocoRGB vetorBlocos[]){
	
	/*path = strtok(path,"/");
	path = strtok(NULL,"/");

	char *nomeArquivo = malloc (sizeof(path)+30);
	strcat(nomeArquivo, "imagens/");
	strcat(nomeArquivo, "com-perdas-");
	strcat(nomeArquivo,path);*/

	FILE *arq = fopen("imagens/descompressaoComPerdas.bmp","wb+");
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
	//free(nomeArquivo);
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