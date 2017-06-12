#pragma once
#include "../tools/Matrix.h"
#include <string>

using std::string;

Matrix<bool>* convertToBool(Matrix<char>& matrix);

Matrix<char>* convertToChar(Matrix<bool>& matrix);

Matrix<char>* readFile(const char* fileName);

void writeFile(const char* fileName, Matrix<char>& matrix);

string* getOutputFileName(char* inputFile, int iterations, const char* tag);