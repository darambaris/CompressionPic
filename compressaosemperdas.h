/*
 ===========================================================================================================
 Nome         : compressaosemperdas.h
 Autores      : Jéssika Darambaris e Roberto Freitas
 Descrição    : Biblioteca de "compressaosemperdas.c".
 ===========================================================================================================
 */

#ifndef COMPRESSAOSEMPERDAS_H
#define COMPRESSAOSEMPERDAS_H




#include <stdio.h>
#include <stdlib.h>
#include "huffman.c"

/*
	Essa função verifica se "val", passado por parâmetro, pertence ao array. Se pertencer, retornará o indice em que val está em array. Caso não encontrar, 
	retornará -1.
*/
int noVetor(unsigned char *, unsigned char , unsigned int );

/*
	Essa função elimina os valores que não são usadas na imagem, para criar um vetor de frequência dos valores que realmente estão presentes na imagem. 
	Os valores usados na imagem e de frequência são retornados por ponteiro. O tamanho desses vetores, cujo tamanho é igual, é retornado.
*/
int eliminaAmbcontFreq(unsigned char * , int , unsigned char ** , int ** );

/*
	Essa função grava bit à bit, os códigos de huffman para os valores da imagem. 
	Tudo é salvo em um arquivo binário.
*/
void GravaBitaBit(unsigned char *, unsigned int , struct TabelaHuff * );

/*
	Essa função lê bit a bit do arquivo binário comprimido, e salva o resultado em uma imagem .bmp.
*/
void  LeBitaBit (struct NoArvoreH* , unsigned int );

/*
	Essa é mais uma rotina de descompressão do que uma função em si. É responsável, como pode-se deduzir, por descomprimir o arquivo.
*/
void descompressaoSemPerdas( unsigned char *, int *, unsigned int , unsigned int );

/*
	Analogamente a função anterior, esta, chama uma rotina de compressão da imagem, e logo em seguida, chama a função de descompressão.
*/
int compressaoSemPerdas(unsigned char *, unsigned int );


#endif // COMPRESSAOSEMPERDAS_H