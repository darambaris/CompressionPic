/*
 ===========================================================================================================
 Nome         : compressaosemperdas.c
 Autores      : Jéssika Darambaris e Roberto Freitas
 Descrição    : Arquivo responsável por implementar a técnica de compressão sem perdas Código de Huffman 
 ===========================================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
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


int eliminaAmbcontFreq(unsigned char * val, int tamanho, unsigned char ** intpixel, int ** freq)
{ 

	unsigned char temp[tamanho];
	int tempfreq[tamanho];
	int idx = 0;
	int idxpesq = 0;

	//Evitar problema com 0s
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

void GravaBitaBit(unsigned char *val, unsigned int tamanho, struct TabelaHuff * Tabela)
{
	int idx;
	int sumu=0,sumz=0;
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
				
				
				if(Tabela->codigohuffman[idx][j] == 0)
				{

					sumz++;
					temp = temp <<= 1;
					contaBit ++;
					

				}else if(Tabela->codigohuffman[idx][j] == 1)
				{
					sumu++;
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

	
		if(contaBit != 0)
		{
			temp = temp <<= (8 - contaBit);
			fseek(f,0,SEEK_END);
			fputc(temp,f);
		}

		fclose(f);
}



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
	{
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
		{
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
	
	f = fopen("SemPerda.bmp", "w+");
	fwrite (val, sizeof(char), aux, f);
	fclose(f);

}


void descompressaoSemPerdas( unsigned char *intpixel, int *freq, unsigned int tamanhofreq, unsigned int tamanho){
	struct NoArvoreH * raiz = ArvoreHuffman(intpixel,freq,tamanhofreq);
	LeBitaBit(raiz, tamanho);
}


int compressaoSemPerdas(unsigned char *val, unsigned int tamanho){
	int *freq = NULL;
	unsigned char *intpixel = NULL;
	unsigned int tamanhofreq = 0;

	tamanhofreq = eliminaAmbcontFreq(val,tamanho,&intpixel,&freq);
	struct TabelaHuff* Tabela = TabelaCodigoHuffman(intpixel,freq,tamanhofreq);
	struct NoArvoreH* Arvore = ArvoreHuffman(intpixel,freq,tamanhofreq);
/*
	int sum = 0;
	int idx = 0;
		for(int i =0; i < tamanho; i++)
		{
			idx = noVetor(Tabela->valor, val[i], Tabela->tamanho);
			sum = sum + Tabela->acima[idx];
		}

		printf("%d\n", sum );	
*/	

	GravaBitaBit(val, tamanho, Tabela);

	DestroiTabela(Tabela);

	descompressaoSemPerdas(intpixel, freq, tamanhofreq, tamanho);

	return EXIT_SUCCESS;
}