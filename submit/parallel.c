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
#include <omp.h>
#include "../devKit/Lab3IO.h"
#include "../devKit/timer.h"

#define TOL 0.0005

int main(int argc, char* argv[])
{
    printf("Am i even running?\n");
	int size;
	double** Au;
	double* X;
	double error, Xnorm;
	int* index;
    int thread_count = 1;

    if(argc > 1){
        printf("Thread count received: %s\n", argv[1]);
        thread_count = atoi(argv[1]);
    }

    printf("Thread count set to: %d\n", thread_count);
	Lab3LoadInput(&Au, &size);

	/*Calculate the solution by parallel code*/
    double start; double end;
    

	X = CreateVec(size);
    index = malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i){
        index[i] = i;
    }

    printf("Damn hoe, here we go again.\n");

    GET_TIME(start);
    
    if (size == 1) {
        X[0] = Au[0][1] / Au[0][0];
    } else {
        /*Gaussian elimination*/
        
        for (int k = 0; k < size - 1; ++k){
            printf("%d\n", k);
            /*Pivoting*/
            double yeehaw = 0;
            int j = 0;
            int i;
            # pragma omp parallel for num_threads(thread_count)
            for (i = k; i < size; ++i) {
                if (yeehaw < Au[index[i]][k] * Au[index[i]][k]){
                    yeehaw = Au[index[i]][k] * Au[index[i]][k];
                    j = i;
                }
            }

            if (j != k) /*swap*/ {
                #pragma opm critical
                {
                    i = index[j];
                    index[j] = index[k];
                    index[k] = i;
                }
            }
            
            /*calculating*/
            
            for (int i = k + 1; i < size; ++i) {
                double temp = Au[index[i]][k] / Au[index[k]][k];
                for (j = k; j < size + 1; ++j) {
                    #pragma opm critical
                    {
                        Au[index[i]][j] -= Au[index[k]][j] * temp;
                    }
                }
            }
        }
        
        /*Jordan elimination*/
        for (int k = size - 1; k > 0; --k) {
            for (int i = k - 1; i >= 0; --i ) {
                double temp = Au[index[i]][k] / Au[index[k]][k];
                Au[index[i]][k] -= temp * Au[index[k]][k];
                Au[index[i]][size] -= temp * Au[index[k]][size];
            } 
        }
        
        /*solution*/
        for (int k = 0; k < size; ++k) {
            X[k] = Au[index[k]][size] / Au[index[k]][k];
        }
    }
    GET_TIME(end);
    Lab3SaveOutput(X , size, end - start);
}