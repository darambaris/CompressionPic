#ifndef ARQUIVO_H
#define ARQUIVO_H

	
typedef struct {
     short bfType;       /* Magic number for file */
} NumMagicoBMP;

typedef struct {
     int   bfSize;       /* Size of file */
     short bfReserved1;  /* Reserved */
     short bfReserved2;  /* ... */
     int   bfOffBits;    /* Offset to bitmap data */
} HeaderBMP;

/* bfType deve ser = "MB" */

/**** BMP file info structure ****/
typedef struct {
     int   biSize;          /* Size of info header */
    int            biWidth;         /* Width of image */
    int            biHeight;        /* Height of image */
     short biPlanes;        /* Number of color planes */
     short biBitCount;      /* Number of bits per pixel */
     int   biCompression;   /* Type of compression to use */
     int   biSizeImage;     /* Size of image data */
    int            biXPelsPerMeter; /* X pixels per meter */
    int            biYPelsPerMeter; /* Y pixels per meter */
     int   biClrUsed;       /* Number of colors used */
     int   biClrImportant;  /* Number of important colors */
} InfoHeaderBMP;

/* blocos 8x8 que formam a imagem em RGB*/
typedef struct {
    int r[8][8];
    int g[8][8];
    int b[8][8];
} BlocoRGB;


/*leitura simples em binário para o código de huffman */
unsigned char *leArquivo(unsigned char[], unsigned int *);

/*leitura em blocos de 8x8 pixels levando em consideração o cabeçalho do arquivo BMP */
int *leArquivoEmBlocos(char[], HeaderBMP*, InfoHeaderBMP*,BlocoRGB**,int*);

int escreveArquivo(char,char,int);

int gravaArquivoEmBlocos(char[],HeaderBMP, InfoHeaderBMP,BlocoRGB[]);

void printvetorBlocos(BlocoRGB[],int);

#endif