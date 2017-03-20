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
	int i, j, k, size;
	double** Au;
	double* X;
	double temp, error, Xnorm;
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
    for (i = 0; i < size; ++i){
        index[i] = i;
    }

    printf("Damn hoe, here we go again.\n");

    GET_TIME(start);
    if (size == 1) {
        X[0] = Au[0][1] / Au[0][0];
    } else {
        /*Gaussian elimination*/
         # pragma omp parallel schedule(dynamic) num_threads(thread_count) private(temp, k) shared(Au, Index, size)
         {
            printf("Threads in the team: %d\n", omp_get_num_threads());
            # pragma omp for
            for (k = 0; k < size - 1; ++k){
                /*Pivoting*/
                temp = 0;
                
               
                for (i = k, j = 0; i < size; ++i) {
                    if (temp < Au[index[i]][k] * Au[index[i]][k]){
                        temp = Au[index[i]][k] * Au[index[i]][k];
                        j = i;
                    }
                }

                if (j != k) /*swap*/ {
                    i = index[j];
                    index[j] = index[k];
                    index[k] = i;
                }
                
                /*calculating*/
                for (i = k + 1; i < size; ++i) {                  
                    temp = Au[index[i]][k] / Au[index[k]][k];
                    for (j = k; j < size + 1; ++j) {
                        Au[index[i]][j] -= Au[index[k]][j] * temp;
                    }
                }       
            }
         }
        
        /*Jordan elimination*/
            for (k = size - 1; k > 0; --k) {
                for (i = k - 1; i >= 0; --i ) {
                    temp = Au[index[i]][k] / Au[index[k]][k];
                    Au[index[i]][k] -= temp * Au[index[k]][k];
                    Au[index[i]][size] -= temp * Au[index[k]][size];
                } 
            }

            /*solution*/
            for (k=0; k< size; ++k) {
                X[k] = Au[index[k]][size] / Au[index[k]][k];
            }
        }
    GET_TIME(end);
    Lab3SaveOutput(X , size, end - start);
}