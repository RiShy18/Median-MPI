#ifndef INCLUDED_HEADER_H
#define INCLUDED_HEADER_H

typedef struct{
	int width, height;
} Dimension;

typedef struct {
  unsigned char r,g,b;
} RGB;

RGB * readImage(char* file, int* width, int* height, int* max);
void writeImage(char* file, int width, int height, int max, const RGB *image);

RGB* processImage(int width, int height, RGB *image, int windowSize);
void quickSort(unsigned char *a, int l, int r);
unsigned char median(unsigned char *a, int size);

unsigned char fragment(unsigned char *a, int l, int r);

void printc(char *msg, int color);


#endif
