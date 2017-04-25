#include "Commons.h"
#include<stdio.h>
#include<stdlib.h>

#define ARGV_EXP "Expected arguments: <input_file.dat> <iterations>\n"

#define FILE_NAME_SIZE 256

void checkARGV(int argc, char **argv) {
    if (argc != 3) {
        printf("Expected 2 argv params, but got %d\n", argc - 1);
        printf(ARGV_EXP);
        exit(1);
    }
}

string* getOutputFileName(char* inputFile, int iterations, const char* tag) {
    char* fileName = new char[FILE_NAME_SIZE];
    snprintf(fileName, FILE_NAME_SIZE, "%s_%d_%s.pgm", inputFile, iterations, tag);
    return new string(fileName);
}

void enchance(Matrix<float>& img) {
    /*
    *  Find the max and min absolute values of the array
    */
    float* x = img.ptr();
    int N = img.size();
    float xmin = arrayAbsMin(x, N);
    float xmax = arrayAbsMax(x, N);

    encahnceImg(x, N, xmin, xmax, THRESH);
}
