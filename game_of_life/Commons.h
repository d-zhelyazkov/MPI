#pragma once
#include "../tools/Matrix.h"
#include <string>

using std::string;

#define ITER_NOTIFICATION 100
#define GUI_PROPS 4

#define DIMS 2

void checkARGV(int argc, char **argv);

/*
* Converts the GoL input pattern char data to 0/1 data.
*/
Matrix<char>* convertToBool(Matrix<char>& matrix);

/*
* Converts the 0/1 data to GoL input pattern char data
*/
Matrix<char>* convertToChar(Matrix<char>& matrix);

/*
* Reads the GoL input pattern char data.
* Serial.
*/
Matrix<char>* readFile(string& fileName);

/*
* Reads the GoL output pattern char data.
* Serial.
*/
void writeFile(string& fileName, Matrix<char>& matrix);

/*
* Attaches iterations and tag to the filename.
*/
string* getOutputFileName(string& inputFile, int iterations, const char* tag);

/*
* Extracts width and height from the filename.
*/
void parseWH(string fileName, int& width, int& height);