/*
 ===========================================================================================================
 Nome         : huffman.c
 Autores      : Jéssika Darambaris e Roberto Freitas
 Descrição    : Arquivo responsável por implementar a Árvore de Huffman. 
 ===========================================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
 

// Cria um Heap
struct Heap* CriarHeap(unsigned capacidade)
{
    struct Heap* Heap = (struct Heap*) malloc(sizeof(struct Heap));
    Heap->tamanho = 0; 
    Heap->capacidade = capacidade;
    Heap->array = (struct NoArvoreH**)malloc(Heap->capacidade * sizeof(struct NoArvoreH*));
    return Heap;
}


//Cria um nó da Árvore
struct NoArvoreH* NovoNo (unsigned char val, unsigned freq)
{
    struct NoArvoreH* temp = (struct NoArvoreH*) malloc(sizeof(struct NoArvoreH));
    temp->esqr = temp->dir = NULL;
    temp->val = val;
    temp->freq = freq;
    return temp;
}
 
 
// Função para trocar dois nós
void TrocarNos(struct NoArvoreH** a, struct NoArvoreH** b)
{
    struct NoArvoreH* t = *a;
    *a = *b;
    *b = t;
}
 
//Junta as sub-árvores no Heap 
void HeapMin(struct Heap* Heap, int idx)
{
    int menor = idx;
    int esqr = 2 * idx + 1;
    int dir = 2 * idx + 2;
 
    if (esqr < Heap->tamanho && Heap->array[esqr]->freq < Heap->array[menor]->freq)
      menor = esqr;
 
    if (dir < Heap->tamanho && Heap->array[dir]->freq < Heap->array[menor]->freq)
      menor = dir;
 
    if (menor != idx)
    {
        TrocarNos(&Heap->array[menor], &Heap->array[idx]);
        HeapMin(Heap, menor);
    }
}
 
//Verifica se o Heap tem tamanho um
int TamanhoUm(struct Heap* Heap)
{
    return (Heap->tamanho == 1);
}
 
// Tira o nó com o menor valor do heap
struct NoArvoreH* ValMin (struct Heap* Heap)
{
    struct NoArvoreH* temp = Heap->array[0];
    Heap->array[0] = Heap->array[Heap->tamanho - 1];
    --Heap->tamanho;
    HeapMin(Heap, 0);
    return temp;
}
 
// Função que insere um nó na Árvore.
void Insere(struct Heap* Heap, struct NoArvoreH* NoArvoreH)
{
    ++Heap->tamanho;
    int i = Heap->tamanho - 1;
    while (i && NoArvoreH->freq < Heap->array[(i - 1)/2]->freq)
    {
        Heap->array[i] = Heap->array[(i - 1)/2];
        i = (i - 1)/2;
    }

    Heap->array[i] = NoArvoreH;
}
 
//Constroi Heap
void ConstroiHeap(struct Heap* Heap)
{
    int n = Heap->tamanho - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        HeapMin(Heap, i);
}
 
//Função Verifica se nó é folha.
int Folha (struct NoArvoreH* raiz)
{
    return !(raiz->esqr) && !(raiz->dir) ;
}

//Cria um Heap e adiciona os valores a ele
struct Heap* CriareConstruirHeap (char val[], int freq[], int tamanho)
{
    struct Heap* Heap = CriarHeap(tamanho);
    for (int i = 0; i < tamanho; ++i)
        Heap->array[i] = NovoNo(val[i], freq[i]);

    Heap->tamanho = tamanho;
    ConstroiHeap(Heap);
    return Heap;
}
 
//Principal função para a criação da Arvore de Huffman
struct NoArvoreH* ConstroiArvoredeHuffman (char val[], int freq[], int tamanho)
{
    struct NoArvoreH *esqr, *dir, *acima;
 

    //Passo 1: Criar um Heap.
    struct Heap* Heap = CriareConstruirHeap(val, freq, tamanho);
 
    //Enquanto o Heap não tem apenas um elemento
    while (!TamanhoUm(Heap))
    {
        // Passo 2: Pegar os valores com as duas menores frequencias no heap
        esqr = ValMin(Heap);
        dir = ValMin(Heap);
 

        // Passo 3: Cria um novo nó com frequencia igual a soma da frequencia dos dois nós achados anteriormente.
        acima = NovoNo('$', esqr->freq + dir->freq);
        acima->esqr = esqr;
        acima->dir = dir;
        //  Passo 4: Adiciona o novo nó criado ao Heap.
        Insere(Heap, acima);
    }
 
    // Passo 5: O último elemento do Heap será a raiz da árvore
    return ValMin(Heap);
}

//Monta a Tabela de Huffman 
void MontaTabela (struct NoArvoreH* raiz, int array[], int acima, struct TabelaHuff * Tabela)
{
    
    // Tudo a esquerda será igual a 0
    if (raiz->esqr)
    {
        array[acima] = 0;
        MontaTabela(raiz->esqr, array, acima + 1, Tabela);
    }
 
    // Tudo a direita será igual a 1
    if (raiz->dir){
    
        array[acima] = 1;
        MontaTabela(raiz->dir, array, acima + 1, Tabela);
    }
 
    //Chegou numa folha, então, armazena valor!
    if (Folha(raiz))
    {

        for(int i =0; i < acima; i++ )
            Tabela->codigohuffman[Tabela->tamanho][i] =  array[i];

        Tabela->valor[Tabela->tamanho] = raiz->val;
        Tabela->acima[Tabela->tamanho] =  acima;
        Tabela-> tamanho = (Tabela->tamanho) + 1;
    }
}
 

//Procura pela árvore de Huffman o codigo do arquivo comprimido
struct NoArvoreH * BuscaHuffman (struct NoArvoreH* raiz, unsigned char zou)
{
    
    // Tudo a esquerda será igual a 0
    if (zou == '0')
    {
        //printf("0");
        return raiz->esqr;
    } // Tudo a direita será igual a 1
    else if (zou == '1'){ 
         printf("1");
        return raiz->dir;
    }

}

//Cria a Tabela de Huffman 
struct TabelaHuff * CriaTabela (int tamanho)
{
 
    struct TabelaHuff* Tabela = (struct TabelaHuff*) malloc(sizeof(struct TabelaHuff));
    Tabela->tamanho = 0;
    Tabela->valor = (unsigned char *) malloc ((tamanho + 1)*sizeof(unsigned char));
    Tabela->acima = (int *) malloc ((tamanho + 1)*sizeof(int));
    Tabela->codigohuffman = (int **) malloc((tamanho + 1)*sizeof(int *));
    for (int i = 0; i <= tamanho; i++)
        Tabela->codigohuffman[i] = (int *) malloc (100 * sizeof (int));

    return Tabela;
}

//Destroi a Tabela de Huffman
void DestroiTabela (struct TabelaHuff* Tabela)
{
    for (int i = 0; i < Tabela->tamanho; i++)
    {
        free(Tabela->codigohuffman[i]);
    }
    free(Tabela->codigohuffman);
    free(Tabela->valor);
    free(Tabela->acima);
    free(Tabela);
}


//Escrita
//Cria uma Tabela de Huffman e a retorna.
struct TabelaHuff * TabelaCodigoHuffman (unsigned char val[], int freq[],  unsigned int tamanho)
{
   //  Constroi uma Árvore Huffman
   struct NoArvoreH * raiz = ConstroiArvoredeHuffman(val, freq, tamanho);
   struct TabelaHuff* Tabela = CriaTabela(tamanho);
   int array[MAX_ARV], acima = 0;
   MontaTabela (raiz,array, acima, Tabela);
   return Tabela;

}


//Leitura
//Cria uma Árvore de Huffman, e a retorna.
struct NoArvoreH * ArvoreHuffman (unsigned char val[], int freq[], unsigned int tamanho)
{
   struct NoArvoreH * raiz = ConstroiArvoredeHuffman(val, freq, tamanho);
   return raiz;
}

