/*
 ===========================================================================================================
 Nome         : compressaosemperdas.c
 Autores      : Jéssika Darambaris e Roberto Freitas
 Descrição    : Arquivo responsável por implementar a técnica de compressão sem perdas Código de Huffman 
 ===========================================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "compressaosemperdas.h"
#include "huffman.c"

//Verifica se val está em array[]
int noVetor(unsigned char *array, unsigned char val, unsigned int tamanho)
{
	for(int i =0; i <= tamanho; i++)
	{
		if((array[i] == val))
		{
			return i;
		}
	}
	return -1;
} 

//elimina as cores que nao serão usadas para a codificação da imagem. Cria vetor de frequencias.
int eliminaAmbcontFreq(unsigned char * val, int tamanho, unsigned char ** intpixel, int ** freq)
{ 

	unsigned char temp[tamanho];
	int tempfreq[tamanho];
	int idx = 0;
	int idxpesq = 0;

	//incializa o vetor
	for(int i=0; i<tamanho; i ++)
			temp[i] = -1;

	for(int i = 0; i < tamanho; i++)
		{

			idxpesq = noVetor(temp, val[i], idx); 

			if(idxpesq == -1)
			{
				temp[idx] = (int) val[i];
				tempfreq[idx] = 1;
				idx ++;
			}
			else
			{

				tempfreq[idxpesq] = tempfreq[idxpesq] + 1; 
			}
		}

		//tira o erro da inicialização de unsigned char
		tempfreq[idx] = 0;
		for(int i=0; i < tamanho; i++)
		{
			if(val[i] == -1)
			{
				
				tempfreq[idx] ++; 
			}
		}
		idx++;

		*freq = tempfreq;
		*intpixel = temp;
		return idx;
}

//Grava bit a bit a codificação huffman para os valores extraidos da imagem em um arquivo binário.
void GravaBitaBit(unsigned char *val, unsigned int tamanho, struct TabelaHuff * Tabela)
{
	int idx;
	unsigned char temp = 0b00000000;
	int contaBit = 0;
	FILE *f;
	f = fopen("semperda.dat", "wb+");

	for(int i =0 ; i < tamanho; i++)
	{

		idx = noVetor(Tabela->valor,val[i],Tabela->tamanho);
		
		if(idx != -1)
		{

			for(int j=0; j < Tabela->acima[idx]; j++)
			{
				
				//Armazena bit a bit em um unsigned char que funcionará como buffer.
				if(Tabela->codigohuffman[idx][j] == 0)
				{

					temp = temp <<= 1;
					contaBit ++;
					

				}else if(Tabela->codigohuffman[idx][j] == 1)
				{
					temp = temp <<=1;
					temp = temp | 0b00000001;
					contaBit ++;
					
				}
				if(contaBit == 8)
				{
	
					contaBit = 0;
					fseek(f,0,SEEK_END);
					fputc(temp,f);
					temp = 0b00000000;
		
				}
				
				
			}
		
		}
		else
		{
			//printf("%d \n", val[i]);
		}

		
	}

		//grava os ultimos bits restantes, que nao completaram 1 byte de buffer.
		if(contaBit != 0)
		{
			temp = temp <<= (8 - contaBit);
			fseek(f,0,SEEK_END);
			fputc(temp,f);
		}

		fclose(f);
}


//Le bit a bit do arquivo comprimido, traduz o codigo huffman, e salva a imagem como .bmp
void  LeBitaBit (struct NoArvoreH* raiz, unsigned int tamanho)
{
	unsigned char temp;
	unsigned char *val = (unsigned char *) malloc (sizeof(unsigned char)*tamanho);
	int sumu = 0, sumz = 0;
	struct NoArvoreH *No;
	int aux = 0;
	int count = 0;

	FILE *f;
	f = fopen("semperda.dat", "rb+");

	No = raiz;

	temp  = (unsigned char) fgetc(f);

	while(!feof(f))
	{
		// printf("%d ", temp);
	while(count != 8)
	{	//leitura bit a bit
		if((temp & 0b10000000) == 0b10000000)
		{
			//sumu++;
			temp = temp <<= 1;
			No = BuscaHuffman(No,'1');
			count ++;
		}else
		{
			//sumz++;
			temp =  temp<<=1;
			No = BuscaHuffman(No,'0');
			count++;
		}
		
		if(Folha(No))
		{	//achou nó, então traduz o código huffman
			//printf(":%d\n",No->val );
			val[aux] = No->val;
			No = raiz;
			aux++;
		}
	}

		count = 0;
		temp  = (unsigned char) fgetc(f);
		
	}

	fclose(f);
	//armazena tudo na imagem .bmp
	f = fopen("SemPerda.bmp", "w+");
	fwrite (val, sizeof(char), aux, f);
	fclose(f);

}

//Rotina para a descompressão sem perdas.
void descompressaoSemPerdas( unsigned char *intpixel, int *freq, unsigned int tamanhofreq, unsigned int tamanho){
	struct NoArvoreH * raiz = ArvoreHuffman(intpixel,freq,tamanhofreq);
	LeBitaBit(raiz, tamanho);
}

//Rotina para compressão sem perdas e a chamada da rotina de descompressão, logo após salvar o resultado da compressão.
int compressaoSemPerdas(unsigned char *val, unsigned int tamanho){
	int *freq = NULL;
	unsigned char *intpixel = NULL;
	unsigned int tamanhofreq = 0;

	tamanhofreq = eliminaAmbcontFreq(val,tamanho,&intpixel,&freq);
	struct TabelaHuff* Tabela = TabelaCodigoHuffman(intpixel,freq,tamanhofreq);
	struct NoArvoreH* Arvore = ArvoreHuffman(intpixel,freq,tamanhofreq);

	GravaBitaBit(val, tamanho, Tabela);

	DestroiTabela(Tabela);
	
	//chamada da rotina de descompressão.
	descompressaoSemPerdas(intpixel, freq, tamanhofreq, tamanho);

	return EXIT_SUCCESS;
}
