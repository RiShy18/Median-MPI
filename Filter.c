#include <stdlib.h>
#include <stdio.h>
#include "header.h"
#include <string.h>
#include <math.h>

#define PI 3.14159265

void quickSort(unsigned char *a, int l, int r);
unsigned char fragment(unsigned char *a, int l, int r);
unsigned char median(unsigned char *a, int size);
unsigned char mean(unsigned char *a, int size);
unsigned char gauss(unsigned char *a, int size, int sigma);



RGB *processImage(int width, int height, RGB *image, int imageSize, char* Filter)
{
	int i, j, k, l;
	if (imageSize == 1){
		return image;
	}
	
	// Process all channels per pixel
	RGB *filtered = (RGB*)malloc(height*width*sizeof(RGB));
	for (i=0; i < height; i++){
		for (j=0; j < width; j++){
			unsigned char *channelR = (unsigned char*) malloc(sizeof(unsigned char)*imageSize*imageSize);
			unsigned char *channelB = (unsigned char*) malloc(sizeof(unsigned char)*imageSize*imageSize);
			unsigned char *channelG = (unsigned char*) malloc(sizeof(unsigned char)*imageSize*imageSize);
			
			int start = 0;
			int left = i - imageSize / 2;
			int right = i + imageSize / 2;
			int up = j - imageSize / 2;
			int down = j + imageSize / 2;
			for (k = left; k < right; k++){
				for (l = up; l < down; l++){
					if (k >= 0 && l >= 0 && k < height && l < width){
						channelR[start] = image[k*width + l].r;
						channelG[start] = image[k*width + l].g;
						channelB[start] = image[k*width + l].b;
						start++;
					}
				}
			}

			unsigned char filteredElementR;
			unsigned char filteredElementG;
			unsigned char filteredElementB;

			if (strcmp(Filter, "M")==0){
				quickSort(channelR, 0, start);
				quickSort(channelG, 0, start);
				quickSort(channelB, 0, start);
				filteredElementR = median(channelR, start);
				filteredElementG = median(channelG, start);
				filteredElementB = median(channelB, start);
			}

			if (strcmp(Filter, "A") == 0){
				filteredElementR = mean(channelR, start);
				filteredElementG = mean(channelG, start);
				filteredElementB = mean(channelB, start);
			}
			
			if (strcmp(Filter, "G") == 0){
				filteredElementR = gauss(channelR, start, 5);
				filteredElementG = gauss(channelG, start, 5);
				filteredElementB = gauss(channelB, start, 5);
			}

			
			

			filtered[i*width + j].r = filteredElementR;
			filtered[i*width + j].g = filteredElementG;
			filtered[i*width + j].b = filteredElementB;
		}
	}

	for (i = 0; i < height; i++){
		for (j = 0; j < width; j++){
			image[i*width + j] = filtered[i*width + j];
		}
	}



	return image;

}


void quickSort(unsigned char *a, int l, int r)
{
	int j;

	if( l < r )
	{
		j = fragment( a, l, r);
		quickSort( a, l, j-1);
		quickSort( a, j+1, r);
	}

}



unsigned char fragment(unsigned char *a, int l, int r) {
	unsigned char pivot, t;
	int i, j;
	pivot = a[l];
	i = l; j = r+1;

	while( 1)
	{
		do ++i; while( a[i] <= pivot && i <= r );
		do --j; while( a[j] > pivot );
		if( i >= j ) break;
		t = a[i]; a[i] = a[j]; a[j] = t;
	}
	t = a[l]; a[l] = a[j]; a[j] = t;
	return j;
}


unsigned char median(unsigned char *a, int size){
	int med = 0;
	if(size%2==1){
		med=(a[size/2]+a[(size/2)+1])/2;
	}
	else{
		med= a[size/2];
	}
	unsigned char median = (unsigned char) med;
	return median;
}

unsigned char mean(unsigned char *a, int size){
	int sum = 0;
	for (int i= 0; i < size; i++){
		sum += a[i];
	}
	sum /= size;
	unsigned char mean = (unsigned char) sum;
	return mean;
}

unsigned char gauss(unsigned char* a, int size, int sigma){
	float sum=0;
	for (int i=0; i < size; i++){
		sum= exp( -(a[i])/(2 * sigma * sigma)) / (sigma * sqrt(2*3.141592654));
		unsigned char gauss= (unsigned char) sum;
	}
	unsigned char gauss= (unsigned char) size;
	return gauss;
}