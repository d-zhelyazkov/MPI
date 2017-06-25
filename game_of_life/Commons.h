#pragma once
#include "../tools/Matrix.h"
#include <string>

using std::string;

#define ITER_NOTIFICATION 100

void checkARGV(int argc, char **argv);

Matrix<char>* convertToBool(Matrix<char>& matrix);

Matrix<char>* convertToChar(Matrix<char>& matrix);

Matrix<char>* readFile(string& fileName);

void writeFile(string& fileName, Matrix<char>& matrix);

string* getOutputFileName(string& inputFile, int iterations, const char* tag);

void parseWH(string fileName, int& width, int& height);