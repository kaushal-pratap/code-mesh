#include "matrix/matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Matrix* matrix_create(const char* name, MatrixType type, size_t rows, size_t columns){

    Matrix *matrix = malloc(sizeof(Matrix));
    if(matrix == NULL){
        return NULL;
    }

    matrix->name = name;
    matrix->rows = rows;
    matrix->columns = columns;
    matrix->type = type;

    switch (type){
        case MATRIX_INT:
            matrix->element_size = sizeof(int);
            break;
        
        case MATRIX_FLOAT:
            matrix->element_size = sizeof(float);
            break;

        case MATRIX_DOUBLE:
            matrix->element_size = sizeof(double);
            break;
        
        case MATRIX_CHAR:
            matrix->element_size = sizeof(char);
            break;
    }

    matrix->data = malloc(rows * columns * matrix->element_size);
    if(matrix->data == NULL){
        free(matrix);
        return NULL;
    }

    printf("Matrix Created successfully !!\n Name : %s\n Rows : %zu\n Columns : %zu\n", matrix->name, matrix->rows, matrix->columns);

    return matrix;

}

void matrix_set(Matrix* matrix, size_t index, const void* value){
    switch (matrix->type){
        case MATRIX_INT: {
            int* data = (int*)matrix->data;
            data[index] = *(const int *)value;
            break;
        }
        
        case MATRIX_FLOAT: {
            float* data = (float*)matrix->data;
            data[index] = *(const float *)value;
            break;
        }

        case MATRIX_DOUBLE: {
            double* data = (double*)matrix->data;
            data[index] = *(const double *)value;
            break;
        }
        
        case MATRIX_CHAR: {
            char* data = (char*)matrix->data;
            data[index] = *(const char *)value;
            break;
        }
    }

}

void matrix_get(const Matrix* matrix, size_t index, void* value){
    if (matrix == NULL || value == NULL || index >= matrix->rows * matrix->columns) {
        return;
    }

    switch (matrix->type){
        case MATRIX_INT: {
            const int* data = (const int*)matrix->data;
            *(int *)value = data[index];
            break;
        }
        
        case MATRIX_FLOAT: {
            const float* data = (const float*)matrix->data;
            *(float *)value = data[index];
            break;
        }

        case MATRIX_DOUBLE: {
            const double* data = (const double*)matrix->data;
            *(double *)value = data[index];
            break;
        }
        
        case MATRIX_CHAR: {
            const char* data = (const char*)matrix->data;
            *(char *)value = data[index];
            break;
        }
    }

}
