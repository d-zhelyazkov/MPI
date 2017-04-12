#include <stdio.h>
#include <math.h>
#include "../tools/cio.h"
#include "../tools/Tools.h"

#define ITERATIONS 5000
#define THRESH 255

#define NEIHBOURHOOD_SIZE 4
const char NEIHBOURHOOD[][2] = { {1,0},{ -1,0 },{ 0,1 },{ 0,-1 } };

void enchance(Matrix<float>& img);

int main (int argc, char **argv)
{
    int i,j,k;
  
    Matrix<float>& img = *datread(argv[4]);
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
    pgmwrite(argv[5], img, THRESH);
    
    delete &img;
    return 0;
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
