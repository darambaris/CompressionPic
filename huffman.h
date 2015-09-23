/*
 ===========================================================================================================
 Nome         : huffman.h
 Autores      : Jéssika Darambaris e Roberto Freitas
 Descrição    : Biblioteca de "huffman.c".
 ===========================================================================================================
 */

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>

 //Altura máxima da árvore
#define MAX_ARV 100

/*
    Estrutura de dados para armazenar os nós da Árvore de Huffman.
*/
struct NoArvoreH
{
    unsigned char val;  // Valor do pixel inserido na árvore
    unsigned freq;  // Frequencia do pixel inserido
    struct NoArvoreH *esqr, *dir; // Nós da esquerda e da direita da árvore
};
 
/*
    Essa é a estrutura de dados para o Heap.
    O Heap funcionará como uma fila de prioridades.
*/
struct Heap
{
    unsigned tamanho;    // Tamanho
    unsigned capacidade;   // Capacidade da Árvore
    struct NoArvoreH **array;  // Array de Nós de ArvoreH.
};

/*
    Estrutura de dados para a Tabela de Huffman. Não é necessária, no entanto,
    foi bastante útil na implementação dos algorítimos de compressão.
*/
struct TabelaHuff
{
    unsigned char *valor;       //Valores da tabela de huffman
    int *acima;                //Tamanho 
    int **codigohuffman;       //Codigo do valor, adicionado em vetor
    unsigned int tamanho;      //tamanho da Tabela (numero de valores)
};
 
/*
    Essa função é responsável por instaciar um Heap, que será utilizado na construção da Árvore de Huffman.
    Ela é criada com a "capacidade" enviada por parâmetro, e retorna um ponteiro para o Heap criado.
*/ 
struct Heap* CriarHeap(unsigned capacidade);

/*
    Essa função é responsável por criar os nós da Árvore de Huffman. 
    Para cada nó é associado um valor e uma frequência, passados por parâmetro, que posteriormente será necessário para a construção
    da árvore de Huffman. A função retorn um vetor para um nó criado.
*/
struct NoArvoreH* NovoNo (unsigned char val, unsigned freq);
 
/*
    Essa função é responsável por inverter os nós passados por parâmetro ('a'e 'b').
*/
void TrocarNos(struct NoArvoreH** a, struct NoArvoreH** b);
 
/*
    Essa fuñção arruma o Heap com a sub-árvores. Essa função é essencial para quando a árvore de huffman está sendo
    criada.
*/
void HeapMin(struct Heap* Heap, int idx);
 
/*
    Essa função verifica se o Heap possuí apenas um elemento. Em caso positivo, retorna 1, e retorna 0 em caso negativo.
*/
int TamanhoUm(struct Heap* Heap);
 
/*
    Essa função simplesmente extrai o nó de menor valor do Heap, e o retorna.
*/
struct NoArvoreH* ValMin (struct Heap* Heap);

/*
    Essa função insere os nós no Heap. 
    Função de inicialização do Heap.
*/
void Insere(struct Heap* Heap, struct NoArvoreH* NoArvoreH);
 
/*
    Essa função é utilizada durante a construção do Heap, para, iterativamente, arrumá-lo.
*/
void ConstroiHeap(struct Heap* Heap);

/*
    Essa função verifica se um nó, passado por parâmetro, é uma folha. Caso positivo, a função retorna 1, caso negativo, retorna 0.
*/
int Folha (struct NoArvoreH* raiz);

/*
    Função de inicialização do Heap. Onde este é criado e incializado com os nós da Árvore de Huffman.
*/
struct Heap* CriareConstruirHeap (char val[], int freq[], int tamanho);

/*
    Principal função da Biblioteca. Ela é responsável por criar a árvore de Huffman. A função retorna o nó da raiz da árvore.
*/
struct NoArvoreH* ConstroiArvoredeHuffman (char val[], int freq[], int tamanho);

/*
    Essa função monta uma Tabela de Huffman, com os dados das folhas e caminhos da Árvore de Huffman posteriormente criada.
*/
void MontaTabela (struct NoArvoreH* raiz, int array[], int acima, struct TabelaHuff * Tabela);

/*
    Essa função é recursiva, para identificar um código na Árvore de Huffman, passada por parâmetro.
*/
struct NoArvoreH * BuscaHuffman (struct NoArvoreH* raiz, unsigned char zou);

/*
    Essa função cria uma Tabela de Huffman.
*/
struct TabelaHuff * CriaTabela (int tamanho);

/*
    Essa função é simplesmente responsável por desalocar a Tabela de Huffman.
*/
void DestroiTabela (struct TabelaHuff* Tabela);

/*
    Essa função cria uma Tabela de Huffman, e a retorna. Essa função é mais organizacional mais do que funcional. Retorna o vetor da 
    Tabela criada.
*/
struct TabelaHuff * TabelaCodigoHuffman (unsigned char val[], int freq[],  unsigned int tamanho);


/*
    Analogamente à função "TabelaCodigoHuffman", essa função é responsável por criar uma Árvore de Huffman e retornar seu ponteiro.
*/
struct NoArvoreH * ArvoreHuffman (unsigned char val[], int freq[], unsigned int tamanho);


#endif // HUFFMAN_H