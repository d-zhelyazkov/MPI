#include "Commons.h"
#include <stdio.h>

#define TRUE_CHAR 'O'
#define FALSE_CHAR '.'

Matrix<bool>* convertToBool(Matrix<char>& matrix)
{
    Matrix<bool>* result = new Matrix<bool>(matrix.rows(), matrix.cols());
    for (int i = 0; i < matrix.rows(); i++) {
        for (int j = 0; j < matrix.cols(); j++) {
            //(*result)(i, j) = (matrix(i, j) == TRUE_CHAR);
            switch (matrix(i, j)) {
            case TRUE_CHAR:
                (*result)(i, j) = true;
                break;
            case FALSE_CHAR:
                (*result)(i, j) = false;
                break;
            default:
                printf("wrong char: '%c'", matrix(i, j));
                return NULL;
            }
        }
    }
    return result;
}

Matrix<char>* convertToChar(Matrix<bool>& matrix)
{
    Matrix<char>* result = new Matrix<char>(matrix.rows(), matrix.cols());

    for (int i = 0; i < matrix.rows(); i++) {
        for (int j = 0; j < matrix.cols(); j++) {
            (*result)(i, j) = (matrix(i, j)) ? TRUE_CHAR : FALSE_CHAR;
        }
    }

    return result;
}

Matrix<char>* readFile(const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Cannot open file '%s'.", fileName);
        return NULL;
    }

    int rows, cols;
    fscanf(fp, "%d %d\n", &cols, &rows);
    Matrix<char>* result = new Matrix<char>(rows, cols);

    int buffSize = cols + 2;
    char* buff = new char[buffSize];
    for (int i = 0; i < rows; i++) {
        fgets(buff, buffSize, fp);
        copyArray(result->getRowPtr(i), buff, cols);
    }

    deleteArray(buff);
    fclose(fp);
    return result;
}

void writeFile(const char* fileName, Matrix<char>& matrix) {
    FILE* fp = fopen(fileName, "w");
    if (fp == NULL) {
        printf("Cannot open file '%s'.", fileName);
        return;
    }

    fprintf(fp, "%d %d\n", matrix.cols(), matrix.rows());

    int buffSize = matrix.cols() + 1;
    char* buff = new char[buffSize];
    buff[matrix.cols()] = 0;
    for (int i = 0; i < matrix.rows(); i++) {
        copyArray(buff, matrix.getRowPtr(i), matrix.cols());
        fprintf(fp, "%s\n", buff);
    }

    fclose(fp);
}

#define FILE_NAME_SIZE 256

string* getOutputFileName(char* inputFile, int iterations, const char* tag) {
    char* fileName = new char[FILE_NAME_SIZE];
    snprintf(fileName, FILE_NAME_SIZE, "%s_%d_%s", inputFile, iterations, tag);
    return new string(fileName);
}
