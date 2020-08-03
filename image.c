#include <stdio.h>
//Variables globales
int ancho, alto;
int Rpix, Gpix, Bpix;

void leerImagen(char *nombreArchivo);
void imprimirImagen(int R[ancho][alto], int G[ancho][alto], int B[ancho][alto]);
int main(int argc, char *argv[]){
	leerImagen(argv[1]);
	return 0;
}
void imprimirImagen(int R[ancho][alto], int G[ancho][alto], int B[ancho][alto]){
	FILE *ap;
	ap=fopen("salida.ppm", "w");
	fprintf(ap, "P3\n%d %d\n255\n",ancho,alto);
	int i,j;
	for(i=0;i<alto;i++){
		for(j=0;j<ancho;j++){
			fprintf(ap, "%d %d %d ", R[i][j],G[i][j],B[i][j]);
		}
		fprintf(ap,"\n");
	}
	fclose(ap);
}

void clasifica(int R[ancho][alto], int G[ancho][alto], int B[ancho][alto]){
	int i,j;
	for(i=0;i<alto;i++){
		for(j=0;j<ancho;j++){
			int test=G[i][j];
			printf("Gpix: %d\n",test);
			if (B[i][j]<R[i][j]){
				if(G[i][j]<R[i][j]){
					
					Rpix++;
				}
			}
			else if (R[i][j]<B[i][j]){
				if(G[i][j]<B[i][j]){

					Bpix++;

				}
			}
			else{

				Gpix++;

			}
		}
	}
	printf("Rpix: %d\n",Rpix );
	printf("Bpix: %d\n",Bpix );
	printf("Gpix: %d\n",Gpix );
}

void leerImagen(char *nombreArchivo){
	FILE *archivo;
	archivo = fopen(nombreArchivo, "r");
	char letra; int tipo, maxcolor;
	fscanf(archivo, "%c%d %d %d %d", &letra, &tipo, &ancho, &alto, &maxcolor);
	int R[ancho][alto],G[ancho][alto],B[ancho][alto];
	int i,j;
	for(i=0;i<alto;i++){
		for(j=0;j<ancho;j++){
			fscanf(archivo, "%d %d %d", &R[i][j],&G[i][j],&B[i][j]);
		}
	}
	fclose(archivo);
	imprimirImagen(R,G,B);
	clasifica(R,G,B);
}
