#include <stdio.h>
#include <math.h>
#include <time.h>
#include "../tools/cio.h"
#include "../tools/Tools.h"

#define ITERATIONS 5000
#define THRESH 255

void enchance(Matrix<float>& img);

int main (int argc, char **argv)
{
    clock_t beginTime = clock();

    int i,j,k;
  
    Matrix<float>& img = *datread(argv[1]);
    int m = img.rows();
    int n = img.cols();
    printf("Image read.\n");
    clock_t readTime = clock() - beginTime;

    Matrix<float> processedImg(m + 2, n + 2);
    for (i = 0; i < m; i++) {
        float* imgRow = img.getRowPtr(i);
        float* processRow = &processedImg.getRowPtr(i + 1)[1];
        copyArray(processRow, imgRow, n);
    }
    printf("currImg created - ");
    print(processedImg);

    Matrix<float> buffImg(m + 2, n + 2);
    printf("buffImg created - ");
    print(buffImg);

    clock_t initTime = clock() - readTime;

    printf("Start processing.\n");
    double avgItTime = 0;
    for (k = 1; k <= ITERATIONS; k++) {
        clock_t itStartTime = clock();
        for (i = 1; i <= m; i++) {
            for (j = 1; j <= n; j++)
            {
                buffImg(i, j) = 0.25 * (processedImg(i-1,j) + processedImg(i + 1, j) +
                    processedImg(i, j - 1)+ processedImg(i, j + 1) + img(i - 1, j - 1));
            }
        }

        clock_t itProcessTime = clock() - itStartTime;
        avgItTime = (double)itProcessTime / ITERATIONS;

        processedImg = buffImg;
	    if (!(k % 1000))
            printf("%d iterations done\n", k);
    }
    clock_t processTime = clock() - initTime;

    enchance(processedImg);
    pgmwrite(argv[2], processedImg, THRESH);
    printf("Image written.\n");

    clock_t writeTime = clock() - processTime;

    printf("Iteration: %f\n", avgItTime);

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
