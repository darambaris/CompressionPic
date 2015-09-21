/*
 ============================================================================
 Nome         : menu.c
 Autores      : Jéssika Darambaris e Roberto Freitas
 Descrição    : Arquivo responsável pela aplicação das transformadas DCT
 ============================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include "dct.h"
#define PI 3.14159265
#define N 8

/* a matriz de quantização utilizada é a típica de luminância para jpeg */
int matrizQuantizacao[8][8] = {
	{ 16, 11, 10, 16,  24,  40,  51,  61 },
	{ 12, 12, 14, 19,  26,  58,  60,  55 },
	{ 14, 13, 16, 24,  40,  57,  69,  56 },
	{ 14, 17, 22, 29,  51,  87,  80,  62 },
	{ 18, 22, 37, 56,  68, 109, 103,  77 },
	{ 24, 35, 55, 64,  81, 104, 113,  92 },
	{ 49, 64, 78, 87, 103, 121, 120, 101 },
	{ 72, 92, 95, 98, 112, 100, 103,  99 }
};

/* Obs: Muitos coeficientes apresentarão valores próximos a zero, podendo ser 
eliminados sem que haja perda significativa de informações contidas na imagem */

void deslocamentoDeNivel(int bloco[8][8], int nivel) {
	int i, j;

	for (i=0;i<8;i++){
		for (j=0;j<8;j++){
			bloco[i][j] += nivel; // (256 cores)/2
		}
	}
}

/* função alfa da fórmula */
double funcao_alfa (int index) {
	if (index == 0) 
		return (double) (1.0/ (double)(2.0 * sqrt(2))); // sqrt(2/8) = 1 / sqrt(2)*2
	else
		return 0.5; // sqrt(1/8)
}

/* aplica a transformada DCT nos dados espaciais por bloco de 8x8 pixels */
void aplicaTransformadaDCT(int bloco[8][8]) {
	int i,j,x,y;
	
	deslocamentoDeNivel(bloco,-127);
	
	double aux[8][8];
	
	// aplica as equações para DCT direta no bloco 
	for(i=0;i<8;i++) {
		for(j=0;j<8;j++) {
			double sum = 0;
			for (x = 0; x < 8; x++) {
				for (y = 0; y < 8; y++) {
					double cos1 = cos(((2.0*x+1.0)*(i*PI))/16.0);
					double cos2 = cos(((2.0*y+1.0)*(j*PI))/16.0);
					sum += bloco[x][y] *  cos1 * cos2;
				}
			}
			
			//multiplicação dos alfas. 
			aux[i][j] = (funcao_alfa(i) * funcao_alfa(j) * sum) ;

		}
	}

	// salva o resultado no bloco dividindo pela matriz típica
	// erros de arredondamento principalmente causados nessa parte, 
	// são responsáveis pela perda de precisão.
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			bloco[i][j] = aux[i][j] / (double) matrizQuantizacao[i][j];
		}
	}
}


void desfazTransformadaDCT(int bloco[8][8]){
	int i, j, x, y;
	double aux[8][8];

	//para desfazer a transformada deve-se multiplicá-la pela matrizQuantização
	for (i=0; i<8; i++){
		for (j=0; j<8; j++){
			aux[i][j] = bloco[i][j] * (double) matrizQuantizacao[i][j];
		}
	}


	//aplica as equações para transformada inversa DCT em cada bloco
	for (x = 0; x < 8; x++) {
		for (y = 0; y < 8; y++) {
			double sum = 0;
			for (i = 0; i < 8; i++) {
				for (j = 0; j < 8; j++) {
					double cos1 = cos ( ( ( 2.0 * x + 1.0 ) * (i * PI) ) / 16.0 );
					double cos2 = cos ( ( ( 2.0 * y + 1.0 ) * (j * PI) ) / 16.0 );
					sum += (funcao_alfa(i) * funcao_alfa(j) * aux[i][j] *  cos1 * cos2) ;
				}
			}
			bloco[x][y] = sum;
			

			// a quantizacao inversa pode fazer com que -128 vire -129, ou 127 vire 128, por exemplo
			// quando formos salvar a imagem como char ao inves de int, temos alguns probleminhas...
			// para resolver isso, precisamos desse if
			/*if (bloco[x][y] > 127) bloco[x][y] = 127;
			else if (bloco[x][y] < - 128) bloco[x][y] = -128; VER SE SERÁ NECESSÁRIO DEPOIS */
		}
	}

	// desloca novamente o nível para cima
	deslocamentoDeNivel(bloco,127);	
}

int main (int argc, char *argv[]){
	int teste[8][8] = {
	{ 52, 55, 61, 66,  70,  61,  64,  73 },
	{ 63, 59, 55, 90,  109,  85,  69,  72 },
	{ 62, 59, 68, 113,  144,  104,  66,  73 },
	{ 63, 58, 71, 122,  154,  106,  70,  69 },
	{ 67, 61, 68, 104,  126, 88,  68,  70 },
	{ 79, 65, 60, 70,  77, 68,  58,  75 },
	{ 85, 71, 64, 59, 55, 61,  65,  83 },
	{ 87, 79, 69, 68,  65,  76,  78,  94 }
};

	aplicaTransformadaDCT(teste);
	desfazTransformadaDCT(teste);

	int i,j;

	for (i=0;i<8;i++){
		for (j=0;j<8;j++){
			printf("%d ",teste[i][j]);
		}
		printf("\n");
	}
}