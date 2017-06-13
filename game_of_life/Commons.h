#pragma once
#include "../tools/Matrix.h"
#include <string>

using std::string;

void checkARGV(int argc, char **argv);

Matrix<bool>* convertToBool(Matrix<char>& matrix);

Matrix<char>* convertToChar(Matrix<bool>& matrix);

Matrix<char>* readFile(string& fileName);

void writeFile(string& fileName, Matrix<char>& matrix);

string* getOutputFileName(string& inputFile, int iterations, const char* tag);

void parseWH(string fileName, int& width, int& height);