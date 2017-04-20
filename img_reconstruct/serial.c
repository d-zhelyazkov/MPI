#include <stdio.h>
#include <time.h>

int main (int argc, char **argv)
{
   void datread(char *, void *, int , int );
   void pgmwrite(char *, void *, int , int );

   clock_t beginTime = clock();

   int i,j,k;
   int m = 239;
   int n = 432;
   int numit = 5000;
  
   float im[m+2][n+2], old[m+2][n+2], new[m+2][n+2];
   float buf[m][n];

   for (i = 0; i < m+2; i++)
     for (j = 0; j < n+2; j++)
       {
	  im[i][j] = 0.0;
	  old[i][j] = 0.0;
       }

   clock_t initTime = clock() - beginTime;

   datread("edge239x432.dat", buf, m, n);

   for (i = 1; i <= m; i++)
     for (j = 1; j <= n; j++)
       {
	  im[i][j] = buf[i-1][j-1];
	  old[i][j] = buf[i-1][j-1];
       }

   clock_t readTime = clock() - initTime;

   double avgItTime = 0;
   for (k = 1; k <= numit; k++)
     {
       clock_t itStartTime = clock();
	for (i = 1; i <= m; i++)
	  for (j = 1; j <= n; j++)
	    new[i][j] = 0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1]-
			      im[i][j]);
    clock_t itProcessTime = clock() - itStartTime;
    avgItTime = (double)itProcessTime / numit;
	for (i = 1; i <= m; i++)
	  for (j = 1; j <= n; j++)
	    old[i][j] = new[i][j];
	if (!(k % 1000)) printf("%d iterations done\n", k);
     }

   clock_t processTime = clock() - readTime;

   for (i = 1; i <= m; i++)
     for (j = 1; j <= n; j++)
       buf[i-1][j-1] = old[i][j];

   pgmwrite("image.pgm", buf, m, n);
    
   clock_t writeTime = clock() - processTime;

   printf("Iteration: %f\n", avgItTime);

   return 0;
}
