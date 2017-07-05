#pragma once
#include"../tools/Matrix.h"
#include <string>

using std::string;

#define THRESH 255

void checkARGV(int argc, char **argv);

string* getOutputFileName(char* inputFile, int iterations, const char* tag);

/*
*  Find the max and min absolute values of the array
*/
float arrayAbsMin(float* arr, int N);

float arrayAbsMax(float* arr, int N);

void encahnceImg(float* img, int size, float min, float max, float thresh);

void enchance(Matrix<float>& img);