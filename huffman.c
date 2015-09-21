/*
 ===========================================================================================================
 Nome         : huffman.c
 Autores      : Jéssika Darambaris e Roberto Freitas
 Descrição    : Arquivo responsável por implementar a árvore de Huffman. 
 ===========================================================================================================
 */


#include <stdio.h>
#include <stdlib.h>

 //Altura máxima da árvore
#define MAX_ARV 100
 
//Nó da Árvore de Huffman
struct NoArvoreH
{
    char val;  // Valor do pixel inserido na árvore
    unsigned freq;  // Frequencia do pixel inserido
    struct NoArvoreH *esqr, *dir; // Nós da esquerda e da direita da árvore
};
 
//Coleção de Árvores de Huffman. Funcionará como fila de prioridade.
struct Heap
{
    unsigned tamanho;    // Tamanho
    unsigned capacidade;   // Capacidade da Árvore
    struct NoArvoreH **array;  // Array de Nós de ArvoreH.
};

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
struct NoArvoreH* NovoNo (char val, unsigned freq)
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
 
//Arruma a árvore
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
 

// Função para printar array
void printArray (int array[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", array[i]);
    printf("\n");
}



//Printa o código de Huffman da raíz as folhas. int array[] armazenará os códigos.
void printCodigo(struct NoArvoreH* raiz, int array[], int acima)
{
    // Tudo a esquerda será igual a 0
    if (raiz->esqr)
    {
        array[acima] = 0;
        printCodigo(raiz->esqr, array, acima + 1);
    }
 
    // Tudo a direita será igual a 1
    if (raiz->dir){
    
        array[acima] = 1;
        printCodigo(raiz->dir, array, acima + 1);
    }
 
    //Chegou numa folha, então printar!
    if (Folha(raiz))
    {
        printf("%c: ", raiz->val);
        printArray(array, acima);
    }
}
 
//Faz a codificação Huffman
void CodigoHuffman (char val[], int freq[], int tamanho)
{
   //  Constroi uma Árvore Huffman
   struct NoArvoreH* raiz = ConstroiArvoredeHuffman(val, freq, tamanho);
 
   // Printar os códigos
   int array[MAX_ARV], acima = 0;
   printCodigo(raiz, array, acima);

}

