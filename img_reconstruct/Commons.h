#pragma once
#include"../tools/Matrix.h"
#include <string>

using std::string;

#define THRESH 255

void checkARGV(int argc, char **argv);

string* getOutputFileName(char* inputFile, int iterations, const char* tag);

void enchance(Matrix<float>& img);

