#include <stdio.h>
#include <math.h>
#include "Commons.h"
#include "../tools/cio.h"
#include "../tools/Tools.h"
#include <time.h>

#define THRESH 255


void enchance(Matrix<float>& img);

int main (int argc, char **argv)
{

    checkARGV(argc, argv);

    int i,j,k;
  
    Matrix<float>& img = *datread(argv[1]);

    int m = img.rows();
    int n = img.cols();
    Matrix<float> processedImg(m + 2, n + 2);
    for (i = 0; i < m; i++) {
        float* imgRow = img.getRowPtr(i);
        float* processRow = &processedImg.getRowPtr(i + 1)[1];
        copyArray(processRow, imgRow, n);
    }

    Matrix<float> buffImg(m + 2, n + 2);
    
    printf("Start processing.\n");
    clock_t processBeginTime = clock();
    int iterations = atoi(argv[2]);
    double avgItTime = 0;
    for (k = 1; k <= iterations; k++) {
        clock_t itStartTime = clock();
        for (i = 1; i <= m; i++) {
            for (j = 1; j <= n; j++)
            {
                buffImg(i, j) = 0.25 * (processedImg(i-1,j) + processedImg(i + 1, j) +
                    processedImg(i, j - 1)+ processedImg(i, j + 1) + img(i - 1, j - 1));
            }
        }
        
        processedImg = buffImg;

        clock_t itTime = clock() - itStartTime;
        avgItTime += clockToS(itTime) / iterations;
	    if (!(k % 1000))
            printf("%d iterations done\n", k);
    }
    printf("Processing completed - %fs\n", clockToS(clock() - processBeginTime));
    printf("Avg iteration time: %fs\n", avgItTime);

    clock_t enchanceBeginTime = clock();
    enchance(processedImg);
    printf("Img enchance time: %fs\n", clockToS(clock() - enchanceBeginTime));

    pgmwrite(getOutputFileName(argv[1], iterations, "serial"), processedImg, THRESH);

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
