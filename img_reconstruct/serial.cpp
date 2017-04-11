#include <stdio.h>
#include <math.h>
#include "cio.h"

#define ITERATIONS 5000
#define THRESH 255

void enchance(Matrix<float>& img);

int main (int argc, char **argv)
{

   int i,j,k;
  
   Matrix<float>& buf = *datread("edge239x432.dat");
   int m = buf.rows();
   int n = buf.cols();
   Matrix<float> im(m + 2, n + 2);
   Matrix<float> old(m + 2, n + 2);
   Matrix<float> newIm(m + 2, n + 2);

   for (i = 1; i <= m; i++)
     for (j = 1; j <= n; j++)
       {
	  im(i, j) = old(i, j) = buf(i-1,j-1);
       }

   for (k = 1; k <= ITERATIONS; k++)
     {
	for (i = 1; i <= m; i++)
	  for (j = 1; j <= n; j++)
          newIm(i, j) = 0.25*(old(i-1,j)+old(i+1,j)+old(i,j-1)+old(i,j+1)-
              im(i, j));

    old = newIm;
	if (!(k % 1000)) 
        printf("%d iterations done\n", k);
     }

   for (i = 1; i <= m; i++)
     for (j = 1; j <= n; j++)
       buf(i-1,j-1) = old(i, j);

   enchance(buf);

   pgmwrite("image.pgm", buf, THRESH);
    
   delete &buf;
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
