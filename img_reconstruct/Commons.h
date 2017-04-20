#pragma once
#include<time.h>

void checkARGV(int argc, char **argv);

char* getOutputFileName(char* inputFile, int iterations, char* tag);

double clockToS(clock_t clock);