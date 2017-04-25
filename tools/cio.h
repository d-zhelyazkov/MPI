#pragma once
/*
* This file contains C routines for the MPI Casestudy.
*
* "datread" reads in an edges data file and can be called as follows:
*
*    float buf[M][N];
*    datread("edge.dat", buf, M, N);
*
* "pgmwrite" writes an image as a PGM file, and can be called as follows:
*
*    float buf[M][N];
*    pgmwrite("image.pgm", buf, M, N);
*
*  Note: you MUST link with the maths library -lm to access fabs, sqrt etc.
*/

#include "Matrix.h"


/*
*  Routine to read an "edges" data file into a 2D floating point array
*  x[nx][ny]. Because of the way C handles (or fails to handle!)
*  multi-dimensional arrays we have to cast the pointer to void.
*/
Matrix<float>* datread(const char* filename);

/*
*  Routine to write a PGM image file from a 2D floating point array
*  x[nx][ny]. Because of the way C handles (or fails to handle!)
*  multi-dimensional arrays we have to cast the pointer to void.
*/
void pgmwrite(const char* filename, Matrix<float>& img, float thresh);

