#include <stdio.h>
#include <stdlib.h>

struct Matrix {
	double* arr;
	unsigned int x;
	unsigned int y;
};

/**
 * display a matrix in nice format to read.
 */
void printMatrix (const struct Matrix*);

/**
 * add 2 matricies together
 */
struct Matrix* add(const struct Matrix*, const struct Matrix*);

/**
 * multiply 2 matricies together
 */
struct Matrix* multiply(const struct Matrix*, const struct Matrix*);
