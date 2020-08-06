#include <stdlib.h>
#include "a1.h"
#include <mpi.h>
#include <stdio.h>
#include <time.h>
int main(int argc, char** argv)
{
	
	// parsed image, and filtered image
	RGB *image, *filteredImage;
	
	// image attributes
	int global_width, global_height;
	int width, height, max;
	Dimension *my_dim;
	
	// process information
	int my_rank, p;
	
	// loop variables
	int dest, source;
	int offset;
	
	
	
	// MPI boilerplate
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    //printf("Hello from %s processor %d of %d\n", processor_name, my_rank, p);

	clock_t time;
	time=clock();

	int tag = 0;
	int windowSize = atoi(argv[3]);
	
	// Process 0 parses the PPM file and distributes size attributes
	if (my_rank == 0){
		printf("Hello from %s processor %d of %d\n", processor_name, my_rank, p);
		printc("I am preparing the image\n", 2);
		image = readPPM(argv[1], &global_width, &global_height, &max);
		offset = 0;
		for (dest = 1; dest < p; dest++){
			int rowsToCompute = global_height/p;
			if (dest < global_height % p)
				rowsToCompute++;
			offset += rowsToCompute;
			
			// Need windowSize/2 (or 2*windowSize/2) slack to make sure that the borders are computed correctly
			if (dest == p-1){
				rowsToCompute += (windowSize/2);
			}
			else{
				rowsToCompute += (windowSize/2)*2;
			}
			Dimension *dim = (Dimension*) malloc(sizeof(Dimension));
			dim->width = global_width;
			dim->height = rowsToCompute;
			MPI_Send(dim, 2, MPI_INT, dest, tag, MPI_COMM_WORLD);
		}
		
		// Set own size attributes
		width = global_width;
		height = global_height / p;
		if (global_height % p != 0){
			height += 1;
		}
		height += (windowSize/2);
	}

	// Receive size attributes
	else {	
		printf("Hello from %s processor %d of %d\n", processor_name, my_rank, p);
		printc("I m preparing to process a part of the image\n", 2);
		my_dim = (Dimension*)malloc(sizeof(Dimension));
		MPI_Recv(my_dim, 2, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		width = my_dim->width;
		height = my_dim->height;
		image = (RGB*)malloc(height*width*sizeof(RGB));
	}

	
	// Process 0 distributes rows
	if (my_rank == 0){
		printf("Hello from %s processor %d of %d\n", processor_name, my_rank, p);
		printc("I am preparing the process\n", 2);
		offset = global_height / p;
		if (global_height % p != 0){
			offset += 1;
		}
		offset += (windowSize/2);
		for (dest = 1; dest < p; dest++){
			int rowsToCompute = global_height/p;
			if (dest < global_height % p)
				rowsToCompute++;

			if (dest == p-1){
                                rowsToCompute += (windowSize/2);
                        }
                        else{
                                rowsToCompute += (windowSize/2)*2;
                        }

			if (dest == p-1){
				offset -= (windowSize/2)*2;
			}
			else{
				offset -= (windowSize/2)*2;
			}
		
			
			MPI_Send(image + offset*width, 3*rowsToCompute*width, MPI_UNSIGNED_CHAR, dest, tag, MPI_COMM_WORLD);
			offset += rowsToCompute;
		}		
	}
	
	// Receive rows
	else{
		printf("Hello from %s processor %d of %d\n", processor_name, my_rank, p);
		printc("I have received the chuncks and going to process\n", 3);

		MPI_Recv(image, height*width*3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD, &status);
	}
	
	
	// Process image	
	filteredImage = processImage(width, height, image, windowSize, argv[4]);
	
	
	// Send processed data back to P0
	if (my_rank != 0 && my_rank != p-1){
		printf("Hello from %s processor %d of %d\n", processor_name, my_rank, p);
		printc("Sending Info to P0\n", 4);
		MPI_Send(image + (windowSize/2)*width, (height-2*(windowSize/2))*width*3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
	}
	else if (my_rank == p-1){
		printf("Hello from %s processor %d of %d\n", processor_name, my_rank, p);
		printc("Sending Info to P0\n", 4);
		MPI_Send(image + (windowSize/2)*width, (height-(windowSize/2))*width*3, MPI_UNSIGNED_CHAR, 0, tag, MPI_COMM_WORLD);
	}

	else{
		printf("Hello from %s processor %d of %d\n", processor_name, my_rank, p);
		printc("I am P0 and am receiveing info\n", 6);
		offset = global_height / p;
                if (global_height % p != 0){
                        offset += 1;
                }
		for (source = 1; source < p; source++){
			int rowsToCompute = global_height/p;
			if (source < global_height % p)
				rowsToCompute++;

			MPI_Recv(image+offset*width, rowsToCompute*width*3, MPI_UNSIGNED_CHAR, source, tag, MPI_COMM_WORLD, &status);
			
			offset += rowsToCompute;
		}

	}

	// Process 0 writes processed PPM file
	if (my_rank == 0){
		printf("Hello from %s processor %d of %d\n", processor_name, my_rank, p);
		printc("I writting the new image\n", 1);
		writePPM(argv[2], global_width, global_height, max, filteredImage);
		free(image);
		time=clock()-time;
		double timeTaken = ((double)time)/CLOCKS_PER_SEC;
		printf("Resultado del tiempo %f\n", timeTaken);
		system("ping -c 1 node1");
		//system("ping -c 1 node0");
	}

	MPI_Finalize();
	return(0);
}
