#include <stdlib.h>
#include "header.h"
#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
int main(int argc, char** argv)
{
	
	// entry image, and filtered image
	RGB *image, *filteredImage;
	
	// image attributes
	int globalWidth, globalHeight;
	int width, height, max;
	Dimension *chunk;
	
	// process information
	int myRank, p;
	
	// global send info
	int where, source;
	int offset;

	
	
	// MPI stuff
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    //printf("Hello from %s processor %d of %d\n", processor_name, myRank, p);

	clock_t time;
	

	int tag = 0;
	int imageSize = atoi(argv[3]);
	
	
	// Process 0 parses the image and reads its attributes
	if (myRank == 0){
		time=clock();
		char msg1[150];
		sprintf(msg1, "./conv.sh %s", argv[1]);
		system(msg1);
		printf("Hello from %s processor %d of %d\n", processor_name, myRank, p);
		printc("I am preparing the image\n", 2);
		image = readImage("out.ppm", &globalWidth, &globalHeight, &max);
		offset = 0;
		for (where = 1; where < p; where++){
			int rowsToCompute = globalHeight/p;
			if (where < globalHeight % p)
				rowsToCompute++;
			offset += rowsToCompute;
			
			// Need imageSize/2 (or 2*imageSize/2) , if not, it explodes
			if (where == p-1){
				rowsToCompute += (imageSize/2);
			}
			else{
				rowsToCompute += (imageSize/2)*2;
			}
			Dimension *dim = (Dimension*) malloc(sizeof(Dimension));
			dim->width = globalWidth;
			dim->height = rowsToCompute;
			MPI_Send(dim, 2, MPI_INT, where, tag, MPI_COMM_WORLD);
		}
		
		// Dimentionate a new image of the original image size
		width = globalWidth;
		height = globalHeight / p;
		if (globalHeight % p != 0){
			height += 1;
		}
		height += (imageSize/2);
	}

	// assembly image dimensions per process
	else {	
		printf("Hello from %s processor %d of %d\n", processor_name, myRank, p);
		printc("I m preparing to process a part of the image\n", 2);
		chunk = (Dimension*)malloc(sizeof(Dimension));
		MPI_Recv(chunk, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		width = chunk->width;
		height = chunk->height;
		image = (RGB*)malloc(height*width*sizeof(RGB));
	}

	
	// Process 0 distribute the rows between process p-1 to 1
	if (myRank == 0){
		printf("Hello from %s processor %d of %d\n", processor_name, myRank, p);
		printc("I am preparing the process\n", 2);
		offset = globalHeight / p;
		if (globalHeight % p != 0){
			offset += 1;
		}
		offset += (imageSize/2);
		for (where = 1; where < p; where++){
			int rowsToCompute = globalHeight/p;
			if (where < globalHeight % p)
				rowsToCompute++;

			if (where == p-1){
                                rowsToCompute += (imageSize/2);
                        }
                        else{
                                rowsToCompute += (imageSize/2)*2;
                        }

			if (where == p-1){
				offset -= (imageSize/2)*2;
			}
			else{
				offset -= (imageSize/2)*2;
			}
		
			
			MPI_Send(image + offset*width, 3*rowsToCompute*width, MPI_UNSIGNED_CHAR, where, tag, MPI_COMM_WORLD);
			offset += rowsToCompute;
		}		
	}
	
	// Recieve the chunks from P0
	else{
		printf("Hello from %s processor %d of %d\n", processor_name, myRank, p);
		printc("I have received the chuncks and going to process\n", 3);

		MPI_Recv(image, height*width*3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD, &status);
	}
	
	
	// Filter the image
	filteredImage = processImage(width, height, image, imageSize);
	
	
	// All send progress back de P0
	if (myRank != 0 && myRank != p-1){
		printf("Hello from %s processor %d of %d\n", processor_name, myRank, p);
		printc("Sending Info to P0\n", 4);
		MPI_Send(image + (imageSize/2)*width, (height-2*(imageSize/2))*width*3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
	}
	else if (myRank == p-1){
		printf("Hello from %s processor %d of %d\n", processor_name, myRank, p);
		printc("Sending Info to P0\n", 4);
		MPI_Send(image + (imageSize/2)*width, (height-(imageSize/2))*width*3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
	}

	else{
		printf("Hello from %s processor %d of %d\n", processor_name, myRank, p);
		printc("I am P0 and am receiveing info\n", 6);
		offset = globalHeight / p;
                if (globalHeight % p != 0){
                        offset += 1;
                }
		for (source = 1; source < p; source++){
			int rowsToCompute = globalHeight/p;
			if (source < globalHeight % p)
				rowsToCompute++;

			MPI_Recv(image+offset*width, rowsToCompute*width*3, MPI_UNSIGNED_CHAR, source, tag, MPI_COMM_WORLD, &status);
			
			offset += rowsToCompute;
		}

	}

	// Process 0 writes the new image
	if (myRank == 0){
		printf("Hello from %s processor %d of %d\n", processor_name, myRank, p);
		printc("I writting the new image\n", 1);
		writeImage("output.ppm", globalWidth, globalHeight, max, filteredImage);
		free(image);
		//fclose("output.ppm");
		time=clock()-time;
		double timeTaken = ((double)time)/CLOCKS_PER_SEC;
		double MBsec   = ((double)(imageSize)) * 1000000.0 / (1024.0*1024.0);

		char msg2[100];
		sprintf(msg2, "./redo.sh %s", argv[2]);
		system(msg2);
		char stat1[100];
		sprintf(msg2, "Time summary = %.4f s\n", timeTaken);
		char msg3[100];
		sprintf(msg3, "Global Bandwidth = %.4f Mb/ms\n", MBsec/imageSize);

		printc("-------------------------------------------\n", 5);
		printc("-------------------------------------------\n", 5);
		printc("-------------------------------------------\n", 5);
		printc("-----------------STADISTICS----------------\n", 5);
		printc("-------------------------------------------\n", 5);
		printc("-------------------------------------------\n", 5);
		printc("-------------------------------------------\n", 5);
		printc(msg2, 5);
		printc(msg3, 3);

		//system("rm out.ppm");
		system("rm out.ppm");
		system("rm output.ppm");
		printf("\033[1;31m");
		system("ping -c 3 node1");
		//system("ping -c 1 node0");
	}

	MPI_Finalize();

	

	return(0);
}
