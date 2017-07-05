#include "Commons.h"
#include<stdio.h>
#include<stdlib.h>
#include <math.h>

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

void encahnceImg(float* img, int size, float min, float max, float thresh) {


    for (int i = 0; i < size; i++) {
        /*
        *  Scale the value appropriately so it lies between 0 and thresh
        */
        if (min < 0 || max > thresh)
            img[i] = (int)((thresh*((fabs(img[i] - min)) / (max - min))) + 0.5);
        else
            img[i] = (int)(fabs(img[i]) + 0.5);

        /*
        *  Increase the contrast by boosting the lower values
        */
        img[i] = thresh * sqrt(img[i] / thresh);
    }
}

float arrayAbsMin(float* arr, int N) {

    float min = fabs(arr[0]);

    for (int i = 1; i < N; i++)
    {
        float abs = fabs(arr[i]);
        if (abs < min) min = abs;
    }

    return min;
}

float arrayAbsMax(float* arr, int N) {

    float max = fabs(arr[0]);

    for (int i = 1; i < N; i++)
    {
        float abs = fabs(arr[i]);
        if (abs > max) max = abs;
    }

    return max;
}
