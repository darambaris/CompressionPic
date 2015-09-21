/*
 ===========================================================================================================
 Nome         : menu.c
 Autores      : Jéssika Darambaris e Roberto Freitas
 Descrição    : Arquivo responsável por implementar a técnica de compressão sem perdas Código de Huffman 
 ===========================================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "huffman.c"

//Verifica se val está em array[]
int noVetor(char *array, char val, int tamanho)
{
	for(int i =0; i <= tamanho; i++)
	{
		if(array[i] == val)
		{
			return i;
		}
	}
	return -1;
} 

void printatudo(char *val, int *freq, int tamanho)
{

		for(int i=0; i < tamanho; i ++)
		{
	
		}
}

int eliminaAmbcontFreq(char * val, int tamanho, char ** intpixel, int ** freq)
{ 
	char temp[sizeof(val)];
	int tempfreq[sizeof(val)];
	int idx = 0, idxf = 0;
	int idxpesq = 0;
	
	for(int i = 0; i < tamanho; i++)
		{

			idxpesq = noVetor(temp, val[i], idx); 

			if(idxpesq == -1)
			{
				temp[idx] = val[i];
				tempfreq[idxf] = 1;
				idxf ++;
				idx ++;
			}
			else
			{

				tempfreq[idxpesq] = tempfreq[idxpesq] + 1; 
			}
		}

		*freq = tempfreq;
		*intpixel = temp;
		return idxf;
}


int compressaoSemPerdas(char * val, int tamanho){

	int *freq = NULL;
	char *intpixel = NULL;
	int tamanhofreq = 0;

	tamanhofreq = eliminaAmbcontFreq(val,tamanho,&intpixel,&freq);
	CodigoHuffman(intpixel,freq,tamanhofreq);
	return EXIT_SUCCESS;

}

int main(int argc, char * argv[])
{

    char a[] =  {'a', 'b','c' ,'d' ,'e', 'a', 'b', 'a', 'b','c', 'd'};
    compressaoSemPerdas(a, (int) sizeof(a));
    return 1;
}