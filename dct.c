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
			else if (bloco[x][y] < -128) bloco[x][y] = -128; 
		}
	}

	// desloca novamente o nível para cima
	deslocamentoDeNivel(bloco,128);	
}

int contagemDeZeros(char vetor[64], int *pos){
	int i=63;
	
	/* acha a ultima posição que contém valor != 0 */
	while (i>=0 && vetor[i] == 0) i--;
	*pos = i;
	
	return(63-i);
}

void vetorizaBlocoZigueZague(int bloco[8][8], char **vetor){
 	  
 	   (*vetor)[0]= (char)bloco[0][0];
	   (*vetor)[1]= (char)bloco[0][1];
	   (*vetor)[2]= (char)bloco[1][0];
	   (*vetor)[3]= (char)bloco[2][0];	
	   (*vetor)[4]= (char)bloco[1][1];			
	   (*vetor)[5]= (char)bloco[0][2];
       (*vetor)[6]= (char)bloco[0][3];
	   (*vetor)[7]= (char)bloco[1][2];
	   (*vetor)[8]= (char)bloco[2][1];
	   (*vetor)[9]= (char)bloco[3][0];
	   (*vetor)[10]= (char)bloco[4][0];
	   (*vetor)[11]= (char)bloco[3][1];
	   (*vetor)[12]= (char)bloco[2][2];
	   (*vetor)[13]= (char)bloco[1][3];
	   (*vetor)[14]= (char)bloco[0][4];
	   (*vetor)[15]= (char)bloco[0][5];
	   (*vetor)[16]= (char)bloco[1][4];
	   (*vetor)[17]= (char)bloco[2][3];
	   (*vetor)[18]= (char)bloco[3][2];
	   (*vetor)[19]= (char)bloco[4][1];
	   (*vetor)[20]= (char)bloco[5][0];
	   (*vetor)[21]= (char)bloco[6][0];
	   (*vetor)[22]= (char)bloco[5][1];
	   (*vetor)[23]= (char)bloco[4][2];
	   (*vetor)[24]= (char)bloco[3][3];
	   (*vetor)[25]= (char)bloco[2][4];
	   (*vetor)[26]= (char)bloco[1][5];
	   (*vetor)[27]= (char)bloco[0][6];
	   (*vetor)[28]= (char)bloco[0][7];
	   (*vetor)[29]= (char)bloco[1][6];
	   (*vetor)[30]= (char)bloco[2][5];
	   (*vetor)[31]= (char)bloco[3][4];
	   (*vetor)[32]= (char)bloco[4][3];
	   (*vetor)[33]= (char)bloco[5][2];
	   (*vetor)[34]= (char)bloco[6][1];
	   (*vetor)[35]= (char)bloco[7][0];
	   (*vetor)[36]= (char)bloco[7][1];
	   (*vetor)[37]= (char)bloco[6][2];
	   (*vetor)[38]= (char)bloco[5][3];
	   (*vetor)[39]= (char)bloco[4][4];
	   (*vetor)[40]= (char)bloco[3][5];
	   (*vetor)[41]= (char)bloco[2][6];
	   (*vetor)[42]= (char)bloco[1][7];
	   (*vetor)[43]= (char)bloco[2][7];
	   (*vetor)[44]= (char)bloco[3][6];
	   (*vetor)[45]= (char)bloco[4][5];
	   (*vetor)[46]= (char)bloco[5][4];
	   (*vetor)[47]= (char)bloco[6][3];
	   (*vetor)[48]= (char)bloco[7][2];
	   (*vetor)[49]= (char)bloco[7][3];
	   (*vetor)[50]= (char)bloco[6][4];
	   (*vetor)[51]= (char)bloco[5][5];
	   (*vetor)[52]= (char)bloco[4][6];
	   (*vetor)[53]= (char)bloco[3][7];
	   (*vetor)[54]= (char)bloco[4][7];
	   (*vetor)[55]= (char)bloco[5][6];
	   (*vetor)[56]= (char)bloco[6][5];
	   (*vetor)[57]= (char)bloco[7][4];
	   (*vetor)[58]= (char)bloco[7][5];
	   (*vetor)[59]= (char)bloco[6][6];
	   (*vetor)[60]= (char)bloco[5][7];
	   (*vetor)[61]= (char)bloco[6][7];
	   (*vetor)[62]= (char)bloco[7][6];
	   (*vetor)[63]= (char)bloco[7][7];
}

void desvetorizaBlocoZigueZague(int bloco[8][8],char **vetor){
	//faz o zig zague de volta 
      bloco[0][0]=(int)(*vetor)[0];
	  bloco[0][1]=(int)(*vetor)[1];
	  bloco[1][0]=(int)(*vetor)[2];
	  bloco[2][0]=(int)(*vetor)[3];
      bloco[1][1]=(int)(*vetor)[4];
      bloco[0][2]=(int)(*vetor)[5];
      bloco[0][3]=(int)(*vetor)[6];
      bloco[1][2]=(int)(*vetor)[7];
	  bloco[2][1]=(int)(*vetor)[8];
	  bloco[3][0]=(int)(*vetor)[9];
	  bloco[4][0]=(int)(*vetor)[10];
	  bloco[3][1]=(int)(*vetor)[11];
	  bloco[2][2]=(int)(*vetor)[12];
	  bloco[1][3]=(int)(*vetor)[13];
	  bloco[0][4]=(int)(*vetor)[14];
	  bloco[0][5]=(int)(*vetor)[15];
	  bloco[1][4]=(int)(*vetor)[16];
	  bloco[2][3]=(int)(*vetor)[17];
	  bloco[3][2]=(int)(*vetor)[18];
      bloco[4][1]=(int)(*vetor)[19];
      bloco[5][0]=(int)(*vetor)[20];
      bloco[6][0]=(int)(*vetor)[21];
      bloco[5][1]=(int)(*vetor)[22];
      bloco[4][2]=(int)(*vetor)[23];
      bloco[3][3]=(int)(*vetor)[24];
      bloco[2][4]=(int)(*vetor)[25];
      bloco[1][5]=(int)(*vetor)[26];
      bloco[0][6]=(int)(*vetor)[27];
      bloco[0][7]=(int)(*vetor)[28];
      bloco[1][6]=(int)(*vetor)[29];
      bloco[2][5]=(int)(*vetor)[30];
      bloco[3][4]=(int)(*vetor)[31];
      bloco[4][3]=(int)(*vetor)[32];
      bloco[5][2]=(int)(*vetor)[33];
      bloco[6][1]=(int)(*vetor)[34];
      bloco[7][0]=(int)(*vetor)[35];
      bloco[7][1]=(int)(*vetor)[36];
      bloco[6][2]=(int)(*vetor)[37];
      bloco[5][3]=(int)(*vetor)[38];
      bloco[4][4]=(int)(*vetor)[39];
      bloco[3][5]=(int)(*vetor)[40];
	  bloco[2][6]=(int)(*vetor)[41];
	  bloco[1][7]=(int)(*vetor)[42];
	  bloco[2][7]=(int)(*vetor)[43];
	  bloco[3][6]=(int)(*vetor)[44];
	  bloco[4][5]=(int)(*vetor)[45];
	  bloco[5][4]=(int)(*vetor)[46];
	  bloco[6][3]=(int)(*vetor)[47];
	  bloco[7][2]=(int)(*vetor)[48];
	  bloco[7][3]=(int)(*vetor)[49];
	  bloco[6][4]=(int)(*vetor)[50];
	  bloco[5][5]=(int)(*vetor)[51];
	  bloco[4][6]=(int)(*vetor)[52];
	  bloco[3][7]=(int)(*vetor)[53];
	  bloco[4][7]=(int)(*vetor)[54];
	  bloco[5][6]=(int)(*vetor)[55];
	  bloco[6][5]=(int)(*vetor)[56];
	  bloco[7][4]=(int)(*vetor)[57];
	  bloco[7][5]=(int)(*vetor)[58];
	  bloco[6][6]=(int)(*vetor)[59];
	  bloco[5][7]=(int)(*vetor)[60];
	  bloco[6][7]=(int)(*vetor)[61];
	  bloco[7][6]=(int)(*vetor)[62];
	  bloco[7][7]=(int)(*vetor)[63];	
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