#include "matrix/matrix_detector.h"
#include "matrix/matrix.h"

static enum CXChildVisitResult initializer_visitor(
    CXCursor cursor,
    CXCursor parent,
    CXClientData client_data
){
    MatrixContext* matrix_context = client_data;

    Matrix* matrix = matrix_context->matrix;

    enum CXCursorKind kind = clang_getCursorKind(cursor);
    enum CXCursorKind parentKind = clang_getCursorKind(parent);

    

    if (parentKind == CXCursor_VarDecl && kind != CXCursor_InitListExpr) {
        return CXChildVisit_Continue;
    }

    if(kind == CXCursor_InitListExpr){
        if(parentKind == CXCursor_InitListExpr){
            matrix_context->row += 1;
            matrix_context->column = 0;
        }
        printf("InitListExpr\n");
        return CXChildVisit_Recurse;
    }
    else if(kind == CXCursor_UnexposedExpr){
        return CXChildVisit_Recurse;
    }

    CXEvalResult result = clang_Cursor_Evaluate(cursor);

    if(result == NULL) {
        return CXChildVisit_Recurse;
    }

    CXEvalResultKind resultKind = clang_EvalResult_getKind(result);

    

    // Checking type of literals 

    if(resultKind == CXEval_Int){
        int value = clang_EvalResult_getAsInt(result);
        if (matrix_context->row >= 0) {
            size_t index = (matrix_context->row * matrix->columns) + matrix_context->column;
            matrix_set(matrix, index, &value);
            matrix_context->column++;
        }
        printf("Integer : %d\n", value);
        
        
    }
    else if(resultKind == CXEval_Float){
        double value = clang_EvalResult_getAsDouble(result);
        if (matrix_context->row >= 0) {
            size_t index = (matrix_context->row * matrix->columns) + matrix_context->column;
            matrix_set(matrix, index, &value);
            matrix_context->column++;
        }
        printf("Float : %f\n", value);
    }
    else if(resultKind == CXEval_StrLiteral){
        const char *value = clang_EvalResult_getAsStr(result);
        if (matrix_context->row >= 0) {
            size_t index = (matrix_context->row * matrix->columns) + matrix_context->column;
            matrix_set(matrix, index, &value);
            matrix_context->column++;
        }
        printf("String: %s\n", value);
    }

    clang_EvalResult_dispose(result);

    return CXChildVisit_Recurse;
}

Matrix* matrix_analyze(CXCursor cursor){
    Matrix* matrix = NULL;

    CXType type = clang_getCursorType(cursor);
    if(type.kind != CXType_ConstantArray){
        return NULL;
    }
    if(type.kind == CXType_ConstantArray){
        CXType elemtype = clang_getArrayElementType(type);
        if (elemtype.kind != CXType_ConstantArray) {
            return NULL;
        }
        if(elemtype.kind == CXType_ConstantArray){
            long long rows = clang_getArraySize(type);
            long long columns = clang_getArraySize(elemtype);
            CXString nameStr = clang_getCursorSpelling(cursor);
            printf("Detected 2D Matrix: %s [%lld x %lld]\n", clang_getCString(nameStr), rows, columns);

            CXType base_type = clang_getArrayElementType(elemtype);
            MatrixType matrix_type;
            switch (base_type.kind)
            {
            case CXType_Int:
                matrix_type = MATRIX_INT;
                break;
            case CXType_Float:
                matrix_type = MATRIX_FLOAT;
                break;
            case CXType_Double:
                matrix_type = MATRIX_DOUBLE;
                break;
            case CXType_Char_S:
                matrix_type = MATRIX_CHAR;
                break;
            default:
                return NULL;
            }
            matrix = matrix_create(clang_getCString(nameStr), matrix_type, rows, columns);
            clang_disposeString(nameStr);
        }
    }

    MatrixContext matrix_context = {
        .row = -1,
        .column = 0,
        .matrix = matrix
    };

    if(matrix == NULL){
        return NULL;
    }

    clang_visitChildren(
        cursor,
        initializer_visitor,
        &matrix_context
    );

    return matrix;
}


