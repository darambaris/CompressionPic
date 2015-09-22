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
#include "dct.h"
#define PI 3.14159265

/* a matriz de quantização utilizada é a típica de luminância para jpeg */
double matrizQuantizacao[8][8] = {
	{ 16.0, 11.0, 10.0, 16.0,  24.0,  40.0,  51.0,  61.0 },
	{ 12.0, 12.0, 14.0, 19.0,  26.0,  58.0,  60.0,  55.0 },
	{ 14.0, 13.0, 16.0, 24.0,  40.0,  57.0,  69.0,  56.0 },
	{ 14.0, 17.0, 22.0, 29.0,  51.0,  87.0,  80.0,  62.0 },
	{ 18.0, 22.0, 37.0, 56.0,  68.0, 109.0, 103.0,  77.0 },
	{ 24.0, 35.0, 55.0, 64.0,  81.0, 104.0, 113.0,  92.0 },
	{ 49.0, 64.0, 78.0, 87.0, 103.0, 121.0, 120.0, 101.0 },
	{ 72.0, 92.0, 95.0, 98.0, 112.0, 100.0, 103.0,  99.0 }
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
		return (double)  0.5/sqrt(2); // sqrt(1/8) = 1 / sqrt(2)*2
	else
		return 0.5; // sqrt(2/8)
}


/* aplica a transformada DCT nos dados espaciais por bloco de 8x8 pixels */
void aplicaTransformadaDCT(int bloco[8][8]) {
	
	int i,j,x,y;
	
	deslocamentoDeNivel(bloco,-128);
	
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
			bloco[i][j] = (int) (aux[i][j] / matrizQuantizacao[i][j]);
		}
	}
}


void desfazTransformadaDCT(int bloco[8][8]){
	int i, j, x, y;
	double aux[8][8];

	//para desfazer a transformada deve-se multiplicá-la pela matrizQuantização
	for (i=0; i<8; i++){
		for (j=0; j<8; j++){
			aux[i][j] = ((double)bloco[i][j] * (double) matrizQuantizacao[i][j]);
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
			

			// o arredondamento pode ultrapassar os valores estabelecidos
			if (bloco[x][y] > 127) bloco[x][y] = 127;
			else if (bloco[x][y] < - 128) bloco[x][y] = -128; 
		}
	}

	// desloca novamente o nível para cima
	deslocamentoDeNivel(bloco,128);	
}

/*int main (int argc, char *argv[]){
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
}*/