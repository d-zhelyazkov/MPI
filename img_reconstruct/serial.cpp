#include <stdio.h>
#include <math.h>
#include "cio.h"

#define ITERATIONS 5000
#define THRESH 255

#define NEIHBOURHOOD_SIZE 4
const char NEIHBOURHOOD[][2] = { {1,0},{ -1,0 },{ 0,1 },{ 0,-1 } };

void enchance(Matrix<float>& img);

int main (int argc, char **argv)
{
    int i,j,k;
  
    Matrix<float>& img = *datread("edge239x432.dat");
    Matrix<float> old(img);
    int m = img.rows();
    int n = img.cols();
    Matrix<float> newIm(m, n);

    for (k = 1; k <= ITERATIONS; k++) {
        for (i = 0; i < m; i++) {
            for (j = 0; j < n; j++)
            {
                float x = img(i, j);
                for (int l = 0; l < NEIHBOURHOOD_SIZE; l++) {
                    float ci = i + NEIHBOURHOOD[l][0];
                    float cj = j + NEIHBOURHOOD[l][1];
                    x += old(ci, cj);
                }
                newIm(i, j) = 0.25 * x;
            }
        }

        old = newIm;
	    if (!(k % 1000))
            printf("%d iterations done\n", k);
    }

    img = newIm;
    enchance(img);
    pgmwrite("image.pgm", img, THRESH);
    
    delete &img;
    return 0;
}

void enchance(Matrix<float>& img) {
    /*
    *  Find the max and min absolute values of the array
    */
    float* x = img.ptr();
    int N = img.size();
    float xmin = fabs(x[0]);
    float xmax = fabs(x[0]);

    for (int i = 0; i < N; i++)
    {
        if (fabs(x[i]) < xmin) xmin = fabs(x[i]);
        if (fabs(x[i]) > xmax) xmax = fabs(x[i]);
    }

    for (int i = 0; i < N; i++) {
        /*
        *  Scale the value appropriately so it lies between 0 and thresh
        */
        if (xmin < 0 || xmax > THRESH)
            x[i] = (int)((THRESH*((fabs(x[i] - xmin)) / (xmax - xmin))) + 0.5);
        else
            x[i] = (int)(fabs(x[i]) + 0.5);

        /*
        *  Increase the contrast by boosting the lower values
        */
        x[i] = THRESH * sqrt(x[i] / THRESH);
    }
}
