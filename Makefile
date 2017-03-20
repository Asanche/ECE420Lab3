all: dataGen ptester stester

dataGen:
	gcc devKit/datagen.c devKit/Lab3IO.c -o datagen

stester:
	gcc devKit/serialtester.c devKit/Lab3IO.c -o serialtester -lm

ptester:
	gcc -std=c99 submit/parallel.c devKit/Lab3IO.c -o main -lm -fopenmp