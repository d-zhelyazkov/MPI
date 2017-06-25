#include "Commons.h"
#include <stdio.h>

#define TRUE_CHAR 'O'
#define FALSE_CHAR '.'
#define ARGV_EXP "Expected arguments: <input_file.dat> <iterations>\n"

#define FILE_NAME_SIZE 256

void checkARGV(int argc, char **argv) {
    if (argc != 3) {
        printf("Expected 2 argv params, but got %d\n", argc - 1);
        printf(ARGV_EXP);
        exit(1);
    }
}
Matrix<char>* convertToBool(Matrix<char>& matrix)
{
    Matrix<char>* result = new Matrix<char>(matrix.rows(), matrix.cols());
    for (int i = 0; i < matrix.rows(); i++) {
        for (int j = 0; j < matrix.cols(); j++) {
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

Matrix<char>* convertToChar(Matrix<char>& matrix)
{
    Matrix<char>* result = new Matrix<char>(matrix.rows(), matrix.cols());

    for (int i = 0; i < matrix.rows(); i++) {
        for (int j = 0; j < matrix.cols(); j++) {
            (*result)(i, j) = (matrix(i, j)) ? TRUE_CHAR : FALSE_CHAR;
        }
    }

    return result;
}

Matrix<char>* readFile(string& fileName) {
    FILE* fp = fopen(fileName.c_str(), "r");
    if (fp == NULL) {
        printf("Cannot open file '%s'.", fileName.c_str());
        return NULL;
    }

    int rows, cols;
    //fscanf(fp, "%d %d\n", &cols, &rows);
    parseWH(fileName, cols, rows);
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

void writeFile(string& fileName, Matrix<char>& matrix) {
    FILE* fp = fopen(fileName.c_str(), "w");
    if (fp == NULL) {
        printf("Cannot open file '%s'.", fileName.c_str());
        return;
    }

    //fprintf(fp, "%d %d\n", matrix.cols(), matrix.rows());

    int buffSize = matrix.cols() + 1;
    char* buff = new char[buffSize];
    buff[matrix.cols()] = 0;
    for (int i = 0; i < matrix.rows(); i++) {
        copyArray(buff, matrix.getRowPtr(i), matrix.cols());
        fprintf(fp, "%s\n", buff);
    }

    fclose(fp);
}

string* getOutputFileName(string& inputFile, int iterations, const char* tag) {
    char* fileName = new char[FILE_NAME_SIZE];
    snprintf(fileName, FILE_NAME_SIZE, "%s_%d_%s", inputFile.c_str(), iterations, tag);
    return new string(fileName);
}

void parseWH(string fileName, int & width, int & height)
{
    int i = fileName.find_last_of('_');
    string heightStr = fileName.substr(i + 1, fileName.size() - i - 1);
    height = std::stoi(heightStr);

    fileName = fileName.substr(0, i);
    i = fileName.find_last_of('_');
    string widthStr = fileName.substr(i + 1, fileName.size() - i - 1);
    width = std::stoi(widthStr);
}
