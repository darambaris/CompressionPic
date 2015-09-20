#ifndef HUFFMAN_H
#define HUFFMAN_H

// This constant can be avoided by explicitly calculating height of Huffman Tree
#define MAX_TREE_HT 100

#include <stdlib.h>


 
//Estrutura de dados dos nós da Árvore Binária (Huffman).
struct NoHeap
{
    char val;  // Valor do pixel inserido na árvore
    unsigned freq;  // Frequencia do pixel inserido
    struct NoArvoreH *esqr, *dir; // Nós da esquerda e da direita da árvore
};
 
//Estrutura de dados da Árvore Binária (Huffman).
struct Heap
{
    unsigned tamanho;    // Tamanho
    unsigned capacidade;   // Capacidade da Árvore
    struct NoArvoreH **array;  // Array de Nós de ArvoreH.
};
 

/*
    Função que cria um novo nó na Heap.
    - char val: valor da intensidade do pixel
    - freq: frequencia do valor
*/
struct NoHeap* NovoNo (char val, unsigned freq)

/*
    Função que cria um Heap.
    - unsigned capacidade: capacidade da árvore. 
*/
struct Heap* CriarHeap(unsigned capacidade);

/*
    Função para trocar dois nós.
    - NoArvoreH *a: Nó da árvore a. 
    - NoArvoreH *b: Nó da árvore b. 
*/
void TrocarNos(struct NoArvoreH** a, struct NoArvoreH** b);

/*
    Função 
    - struct ArvoreH* ArvoreH: Árvore de Huffman.
    - int idx: index
*/
void ArvoreMin(struct ArvoreH* ArvoreH, int idx);

/*
    Função que verifica se a árvore possui tamanho um.
    - struct ArvoreH* ArvoreH: ArvoreH possui tamanho um?
*/
int TamanhoUm(struct ArvoreH* ArvoreH);

/*
    Função que retira o nó de menor valor da Árvore.
    - struct ArvoreH* ArvoreH: Árvore.
*/
struct NoArvoreH* ValMin (struct ArvoreH* ArvoreH);

/* 
    Função que insere um nó na Árvore.
    - struct ArvoreH* ArvoreH:
    - struct NoArvoreH* NoArvoreH:
*/
void Insere(struct ArvoreH* ArvoreH, struct NoArvoreH* NoArvoreH);

/*
    Função Constroi uma Arvore Binária Huffman
    - struct ArvoreH* ArvoreH: Arvore Binária. 

*/
void ConstroiArvore(struct ArvoreH* ArvoreH);

/*
    Função que gera o Codigo Huffman.
    - char val[]: 
    - int freq[]:
    - int tamanho:
*/
void CodigoHuffman (char val[], int freq[], int tamanho);


/*
    Função que printa Array
    - int array[]: array a ser printado
    - int n: tamanho do array
*/
void printArray (int array[], int n);


#endif // HUFFMAN_H