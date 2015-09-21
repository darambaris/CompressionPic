#ifndef ARQUIVO_H
#define ARQUIVO_H
	
typedef struct {
     short bfType;       /* Magic number for file */
} BMPMAGICNUMBER;

typedef struct {
     int   bfSize;       /* Size of file */
     short bfReserved1;  /* Reserved */
     short bfReserved2;  /* ... */
     int   bfOffBits;    /* Offset to bitmap data */
} HEADERBMP;

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
} INFOHEADERBMP;

/* blocos 8x8 que formam a imagem em RGB*/
typedef struct Bloco {
    int r[8][8];
    int g[8][8];
    int b[8][8];
    struct Bloco *next;
} LISTABLOCOS;


/*leitura simples em binário para o código de huffman */
unsigned char *leArquivo(unsigned char[], unsigned int *);

/*leitura em blocos de 8x8 pixels levando em consideração o cabeçalho do arquivo BMP */
int *leArquivoEmBlocos(char[], HEADERBMP*, INFOHEADERBMP*,LISTABLOCOS*);

int escreveArquivo(char,  char,  int);

#endif