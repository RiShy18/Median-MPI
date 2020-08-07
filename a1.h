#ifndef INCLUDED_A1_H
#define INCLUDED_A1_H

typedef struct{
	int width, height;
} Dimension;

typedef struct {
  unsigned char r,g,b;
} RGB;

RGB * readPPM(char* file, int* width, int* height, int* max);
void writePPM(char* file, int width, int height, int max, const RGB *image);

RGB* processImage(int width, int height, RGB *image, int windowSize);
void quickSort(unsigned char *a, int l, int r);
unsigned char median(unsigned char *a, int size);

unsigned char partition(unsigned char *a, int l, int r);

void printc(char *msg, int color);


#endif
