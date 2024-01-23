#include "linear-algebra.h"
#include <stdio.h>

void printMatrix(const struct Matrix* matrixPtr){
	for (int i = 0; i < matrixPtr->y; i++){
		printf("| ");
		for (int j = 0; j < matrixPtr->x; j++){
			printf("%.3f ", matrixPtr->arr[i*matrixPtr->x+j]);
		}
		printf("|\n");
	}
}

struct Matrix* add (const struct Matrix* matrix1, const struct Matrix* matrix2){
	if (matrix1->x != matrix2->x && matrix1->y != matrix2->y){ 
		perror("the matrix is not of the same size");
		return NULL;
	}
	struct Matrix* matrix = (struct Matrix*) malloc(sizeof(struct Matrix));
	matrix->x = matrix1->x;
	matrix->y = matrix2->y;
	matrix->arr = (double*) malloc(matrix1->x * matrix2->y * sizeof(double));
	for (int i = 0; i < matrix1->x * matrix2->y; i++){
		matrix->arr[i] = matrix1->arr[i] + matrix2->arr[i];
	}
	return matrix;
}

struct Matrix* multiply (const struct Matrix* matrix1, const struct Matrix* matrix2){
	if (matrix1->x!=matrix2->y) {
		perror("size error");
		return NULL;
	}
	struct Matrix* matrix = (struct Matrix*) malloc(sizeof(struct Matrix));
	matrix->x = matrix2->x;
	matrix->y = matrix1->y;
	matrix->arr = (double*) malloc(matrix->x * matrix->y * sizeof(double));
	for (int x = 0; x < matrix->x; x++){
		for (int y = 0; y < matrix->y; y++){
			matrix->arr[x + y * matrix->x] = 0;
			for (int i = 0; i < matrix1->x; i++){
				matrix->arr[x + y * matrix->x] += matrix1->arr[i + y * matrix1->x] * matrix2->arr[x + i * matrix2->x];
			}
		}
	}
	return matrix;
}

int main(){
	struct Matrix matrix1;
	matrix1.x=2;
	matrix1.y=2;
	matrix1.arr = (double*) malloc(4*sizeof(double));
	matrix1.arr[0] = 1;
	matrix1.arr[1] = 2;
	matrix1.arr[2] = 3;
	matrix1.arr[3] = 4;
	struct Matrix matrix2;
	matrix2.x=2;
	matrix2.y=2;
	matrix2.arr = (double*) malloc(4*sizeof(double));
	matrix2.arr[0] = 5;
	matrix2.arr[1] = 6;
	matrix2.arr[2] = 7;
	matrix2.arr[3] = 8;
	printMatrix(multiply(&matrix1, &matrix2));
	return 1;
}
