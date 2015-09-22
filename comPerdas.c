/*
 ============================================================================================================
 Nome         : CompressaoComPerdas.c
 Autores      : Jéssika Darambaris e Roberto Freitas
 Descrição    : Arquivo responsável por implementar a técnica de compressão transformada dos cossenos (DCT)
 ============================================================================================================
 */
#include <stdio.h>
#include <stdlib.h> 

#include "comPerdas.h"
#include "dct.h"

int compressaoComPerdas(BlocoRGB **vetorBlocos, int qtdeBlocos){
	int i;

	for (i=0;i<qtdeBlocos;i++){
		aplicaTransformadaDCT((*vetorBlocos)[i].r);
		aplicaTransformadaDCT((*vetorBlocos)[i].g);
		aplicaTransformadaDCT((*vetorBlocos)[i].b);
	}
	
	return EXIT_SUCCESS;	
}


int descompressaoComPerdas(BlocoRGB **vetorBlocos, int qtdeBlocos){
	int i;

	for (i=0;i<qtdeBlocos;i++){
		desfazTransformadaDCT((*vetorBlocos)[i].r);
		desfazTransformadaDCT((*vetorBlocos)[i].g);
		desfazTransformadaDCT((*vetorBlocos)[i].b);
	}

	return EXIT_SUCCESS;
}

