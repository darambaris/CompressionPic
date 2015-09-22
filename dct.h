#ifndef DCT_H
#define DCT_H
	void aplicaTransformadaDCT(int[8][8]);
	void desfazTransformadaDCT(int[8][8]);
	double funcao_alfa(int);
	int contagemDeZeros(int[64], int *);
	void vetorizaBlocoZigueZague(int[8][8], int**);
#endif