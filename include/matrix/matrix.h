#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Matrix Structure for Pure Linear Algebra Math Operations
 */
typedef struct {
    char name[64];
    size_t rows;
    size_t cols;
    double *data;
} Matrix;

Matrix *matrix_create(const char *name, size_t rows, size_t cols);
void matrix_free(Matrix *m);

double matrix_get(const Matrix *m, size_t row, size_t col);
bool matrix_set(Matrix *m, size_t row, size_t col, double val);

Matrix *matrix_add(const Matrix *A, const Matrix *B);
Matrix *matrix_multiply(const Matrix *A, const Matrix *B);
Matrix *matrix_transpose(const Matrix *A);

#endif // MATRIX_H
