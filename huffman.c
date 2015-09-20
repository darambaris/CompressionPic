/*
 ===========================================================================================================
 Nome         : huffman.c
 Autores      : Jéssika Darambaris e Roberto Freitas
 Descrição    : Arquivo responsável por implementar a árvore de Huffman. 
 ===========================================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#define MAX_TREE 100
 
//Estrutura de dados dos nós da Árvore Binária (Huffman).
struct NoHeap
{
    char val;  // Valor do pixel inserido na árvore
    unsigned freq;  // Frequencia do pixel inserido
    struct NoHeap *esqr, *dir; // Nós da esquerda e da direita da árvore
};
 
//Estrutura de dados da Árvore Binária (Huffman).
struct Heap
{
    unsigned tamanho;    // Tamanho
    unsigned capacidade;   // Capacidade da Árvore
    struct NoHeap **array;  // Array de Nós de ArvoreH.
};

// Cria um heap
struct Heap* CriarHeap(unsigned capacidade)
{
    struct Heap* Heap = (struct Heap*) malloc(sizeof(struct Heap));
    Heap->tamanho = 0; 
    Heap->capacidade = capacidade;
    Heap->array = (struct NoHeap**)malloc(Heap->capacidade * sizeof(struct NoHeap*));
    return Heap;
}


//Cria um nó do Heap
struct NoHeap* NovoNo (char val, unsigned freq)
{
    struct NoHeap* temp = (struct NoHeap*) malloc(sizeof(struct NoHeap));
    temp->esqr = temp->dir = NULL;
    temp->val = val;
    temp->freq = freq;
    return temp;
}
 
 
// Função para trocar dois nós
void TrocarNos(struct NoHeap** a, struct NoHeap** b)
{
    struct NoHeap* t = *a;
    *a = *b;
    *b = t;
}
 

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
 
//Verifica se a Árvore tem tamanho um
int TamanhoUm(struct Heap* Heap)
{
    return (Heap->tamanho == 1);
}
 
// Tira o nó com o menor valor do heap
struct NoHeap* ValMin (struct Heap* Heap)
{
    struct NoHeap* temp = Heap->array[0];
    Heap->array[0] = Heap->array[Heap->tamanho - 1];
    --Heap->tamanho;
    HeapMin(Heap, 0);
    return temp;
}
 
// Função que insere um nó na Árvore.
void Insere(struct Heap* Heap, struct NoHeap* NoHeap)
{
    ++Heap->tamanho;
    int i = Heap->tamanho - 1;
    while (i && NoHeap->freq < Heap->array[(i - 1)/2]->freq)
    {
        Heap->array[i] = Heap->array[(i - 1)/2];
        i = (i - 1)/2;
    }

    Heap->array[i] = NoHeap;
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
int Folha (struct NoHeap* raiz)
{
    return !(raiz->esqr) && !(raiz->dir) ;
}

//Cria um Heap para armazena os valores
struct Heap* CriareConstruirHeap (char val[], int freq[], int tamanho)
{
    struct Heap* Heap = CriarHeap(tamanho);
    for (int i = 0; i < tamanho; ++i)
        Heap->array[i] = NovoNo(val[i], freq[i]);

    Heap->tamanho = tamanho;
    ConstroiHeap(Heap);
    return Heap;
}
 
// The main function that builds Huffman tree
struct NoHeap* ConstroiArvoredeHuffman (char val[], int freq[], int tamanho)
{
    struct NoHeap *esqr, *dir, *acima;
 
    // Step 1: Create a min heap of capacity equal to size.  Initially, there are
    // modes equal to size.
    struct Heap* Heap = CriareConstruirHeap(val, freq, tamanho);
 
    // Iterate while size of heap doesn't become 1
    while (!TamanhoUm(Heap))
    {
        // Step 2: Extract the two minimum freq items from min heap
        esqr = ValMin(Heap);
        dir = ValMin(Heap);
 
        // Step 3:  Create a new internal node with frequency equal to the
        // sum of the two nodes frequencies. Make the two extracted node as
        // left and right children of this new node. Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        acima = NovoNo('$', esqr->freq + dir->freq);
        acima->esqr = esqr;
        acima->dir = dir;
        Insere(Heap, acima);
    }
 
    // Step 4: The remaining node is the root node and the tree is complete.
    return ValMin(Heap);
}
 

// A utility function to print an array of size n
void printArray (int array[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", array[i]);
    printf("\n");
}

// Prints huffman codes from the root of Huffman Tree.  It uses arr[] to
// store codes
void printCodigo(struct NoHeap* raiz, int array[], int acima)
{
    // Assign 0 to left edge and recur
    if (raiz->esqr)
    {
        array[acima] = 0;
        printCodigo(raiz->esqr, array, acima + 1);
    }
 
    // Assign 1 to right edge and recur
    if (raiz->dir)
    {
        array[acima] = 1;
        printCodigo(raiz->dir, array, acima + 1);
    }
 
    // If this is a leaf node, then it contains one of the input
    // characters, print the character and its code from arr[]
    if (Folha(raiz))
    {
        printf("%c: ", raiz->val);
        printArray(array, acima);
    }
}
 
//Faz a codificação Huffman
void CodigoHuffman (char val[], int freq[], int tamanho)
{
   //  Construct Huffman Tree
   struct NoHeap* raiz = ConstroiArvoredeHuffman(val, freq, tamanho);
 
   // Print Huffman codes using the Huffman tree built above
   int array[MAX_TREE], acima = 0;
   printCodigo(raiz, array, acima);
}