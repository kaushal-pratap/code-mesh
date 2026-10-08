#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include <stdbool.h>

typedef enum{
    MATRIX_INT,
    MATRIX_FLOAT,
    MATRIX_DOUBLE,
    MATRIX_CHAR
}MatrixType;

typedef struct{
    const char *name;
    size_t rows;
    size_t columns;
    MatrixType type;
    size_t element_size;
    void *data;
}Matrix;

typedef struct{
    Matrix* matrix;
    size_t row;
    size_t column;
}MatrixContext;

Matrix* matrix_create(const char* name, MatrixType type, size_t rows, size_t columns);
void matrix_set(Matrix* matrix, size_t index, const void* value);
void matrix_get(const Matrix* matrix, size_t index, void* value);

#endif // MATRIX_H
