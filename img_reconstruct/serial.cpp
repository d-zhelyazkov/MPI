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
  
    Matrix<float>& img = *datread(argv[1]);
    printf("Image read.\n");

    Matrix<float> currImg(img);
    printf("currImg created - ");
    print(currImg);

    int m = img.rows();
    int n = img.cols();
    Matrix<float> buffImg(m, n);
    printf("buffImg created - ");
    print(buffImg);

    printf("Start processing.\n");
    for (k = 1; k <= ITERATIONS; k++) {
        for (i = 0; i < m; i++) {
            for (j = 0; j < n; j++)
            {
                float x = img(i, j);
                for (int l = 0; l < NEIHBOURHOOD_SIZE; l++) {
                    float ci = i + NEIHBOURHOOD[l][0];
                    float cj = j + NEIHBOURHOOD[l][1];
                    x += currImg(ci, cj);
                }
                buffImg(i, j) = 0.25 * x;
            }
        }

        currImg = buffImg;
	    if (!(k % 1000))
            printf("%d iterations done\n", k);
    }

    img = currImg;
    enchance(img);
    pgmwrite(argv[2], img, THRESH);
    printf("Image written.\n");

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
