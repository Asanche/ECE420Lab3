//Remember to change the exit value of the wrong case to -1 !!!
/*
-----
Compiling:
    "Lab3IO.c" should be included and "-lm" tag is needed, like
    > gcc parallel.c Lab3IO.c -o serialtester -lm
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../devkit/Lab3IO.h"
#include "../devKit/Timer.h"

#define TOL 0.0005

int main(int argc, char* argv[])
{
    printf("Am i even running?");
	int i, j, k, size;
	double** Au;
	double* X;
	double temp, error, Xnorm;
	int* index;

	Lab3LoadInput(&Au, &size);

	/*Calculate the solution by parallel code*/
    double start; double end;
    GET_TIME(start);

	X = CreateVec(size);
    index = malloc(size * sizeof(int));
    for (i = 0; i < size; ++i){
        index[i] = i;
    }

    printf("Damn hoe, here we go again.");

    if (size == 1) {
        X[0] = Au[0][1] / Au[0][0];
    } else {
        /*Gaussian elimination*/
        # pragma omp parallel shared(k, Au, index) pivate (k, i, j, temp) default(none)
        {
            for (k = 0; k < size - 1; ++k){
                /*Pivoting*/
                printf("Pivot that shit, dawg.");
                temp = 0;
        #       pragma omp for 
                for (i = k, j = 0; i < size; ++i) {
                    printf("pragma 1");
                    if (temp < Au[index[i]][k] * Au[index[i]][k]){
                        temp = Au[index[i]][k] * Au[index[i]][k];
                        j = i;
                    }
                }

                if (j != k) /*swap*/ {
                    printf("Swap that shit, dawg");
                    i = index[j];
                    index[j] = index[k];
                    index[k] = i;
                }
                
                /*calculating*/
        #       pragma omp for 
                for (i = k + 1; i < size; ++i) {
                    printf("pragma 2");                    
                    temp = Au[index[i]][k] / Au[index[k]][k];
                    for (j = k; j < size + 1; ++j) {
                        Au[index[i]][j] -= Au[index[k]][j] * temp;
                    }
                }       
            }
        }
        
        /*Jordan elimination*/
        # pragma omp parallel shared(Au, index, size, X) private(temp, k) default(none)
        {
        #   pragma omp for
            for (k = size - 1; k > 0; --k) {
                printf("pragma 3");
                for (i = k - 1; i >= 0; --i ) {
                    temp = Au[index[i]][k] / Au[index[k]][k];
                    Au[index[i]][k] -= temp * Au[index[k]][k];
                    Au[index[i]][size] -= temp * Au[index[k]][size];
                } 
            }
        

        /*solution*/
        #   pragma omp for 
            for (k=0; k< size; ++k) {
                printf("pragma 4");
                X[k] = Au[index[k]][size] / Au[index[k]][k];
            }
        }
    }
    GET_TIME(end);
    Lab3SaveOutput(X , size, end - start);
}