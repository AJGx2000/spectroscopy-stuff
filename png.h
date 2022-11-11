#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>

typedef struct {
    unsigned int length;
    char *type;
    unsigned char *data;
    unsigned int crc;
} chunk;

typedef struct {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
} RGBA;

typedef struct {
    unsigned char magic[8];
    chunk *chunks;
    int height;
    int width;
    int numChunks;
    chunk compressed;
    chunk decompressed;
    RGBA **pixels;
} png;

int cap(unsigned int n);
int cap4(unsigned char *p,int i);
chunk nextChunk(FILE *f, int *check);
void printRGBA(png apng, int alpha, int color);
unsigned char PaethPredictor (unsigned char a, unsigned char b, unsigned char c);
png makePNG(char *fName);
png interface(void);
