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

char* getOutputFileName(char* inputFile, int iterations, char* tag) {
    char* fileName = new char[FILE_NAME_SIZE];
    snprintf(fileName, FILE_NAME_SIZE, "%s_%d_%s.pgm", inputFile, iterations, tag);
    return fileName;
}

double clockToS(clock_t clock) {
    return (double)clock / CLOCKS_PER_SEC;
}