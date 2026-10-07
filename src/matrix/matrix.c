#include "matrix/matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Matrix *matrix_create(const char *name, size_t rows, size_t cols) {
    if (rows == 0 || cols == 0) return NULL;
    Matrix *m = (Matrix *)malloc(sizeof(Matrix));
    if (!m) return NULL;

    strncpy(m->name, name ? name : "Matrix", sizeof(m->name) - 1);
    m->rows = rows;
    m->cols = cols;
    m->data = (double *)calloc(rows * cols, sizeof(double));
    if (!m->data) {
        free(m);
        return NULL;
    }
    return m;
}

void matrix_free(Matrix *m) {
    if (m) {
        if (m->data) free(m->data);
        free(m);
    }
}

double matrix_get(const Matrix *m, size_t row, size_t col) {
    if (!m || !m->data || row >= m->rows || col >= m->cols) return 0.0;
    return m->data[row * m->cols + col];
}

bool matrix_set(Matrix *m, size_t row, size_t col, double val) {
    if (!m || !m->data || row >= m->rows || col >= m->cols) return false;
    m->data[row * m->cols + col] = val;
    return true;
}

Matrix *matrix_add(const Matrix *A, const Matrix *B) {
    if (!A || !B || A->rows != B->rows || A->cols != B->cols) return NULL;
    Matrix *res = matrix_create("A + B", A->rows, A->cols);
    if (!res) return NULL;

    for (size_t i = 0; i < A->rows * A->cols; i++) {
        res->data[i] = A->data[i] + B->data[i];
    }
    return res;
}

Matrix *matrix_multiply(const Matrix *A, const Matrix *B) {
    if (!A || !B || A->cols != B->rows) return NULL;
    Matrix *res = matrix_create("A x B", A->rows, B->cols);
    if (!res) return NULL;

    for (size_t r = 0; r < A->rows; r++) {
        for (size_t c = 0; c < B->cols; c++) {
            double sum = 0.0;
            for (size_t k = 0; k < A->cols; k++) {
                sum += matrix_get(A, r, k) * matrix_get(B, k, c);
            }
            matrix_set(res, r, c, sum);
        }
    }
    return res;
}

Matrix *matrix_transpose(const Matrix *A) {
    if (!A) return NULL;
    Matrix *res = matrix_create("A^T", A->cols, A->rows);
    if (!res) return NULL;

    for (size_t r = 0; r < A->rows; r++) {
        for (size_t c = 0; c < A->cols; c++) {
            matrix_set(res, c, r, matrix_get(A, r, c));
        }
    }
    return res;
}
