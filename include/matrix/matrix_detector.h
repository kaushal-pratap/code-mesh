#ifndef MATRIX_DETECTOR_H
#define MATRIX_DETECTOR_H
#include <stdio.h>
#include <clang-c/Index.h>
#include "matrix/matrix.h"

Matrix* matrix_analyze(CXCursor cursor);

#endif