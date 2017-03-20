all: dataGen ptester stester

dataGen:
	gcc devkit/datagen.c devkit/Lab3IO.c -o datagen

stester:
	gcc devkit/serialtester.c devkit/Lab3IO.c -o serialtester -lm

ptester:
	gcc submit/paralleltester.c devkit/Lab3IO.c -o paralleltester -lm