

#include "cio.h"
#include <stdio.h>
#include <stdlib.h>


Matrix<float>* datread(char *filename)
{ 
    FILE *fp;
    int i, j, t;

    if (NULL == (fp = fopen(filename,"r")))
        {
	fprintf(stderr, "datread: cannot open <%s>\n", filename);
	exit(-1);
        }

    int nx, ny;
    fscanf(fp, "%d %d", &nx, &ny);
    Matrix<float>& vx = *new Matrix<float>(ny , nx);

    /*
    *  Must cope with the fact that the storage order of the data file
    *  is not the same as the storage of a C array, hence the pointer
    *  arithmetic to access x[i][j].
    */
    for (j = 0; j < ny; j++)
        for (i = 0; i < nx; i++)
        {
	    fscanf(fp, "%d", &t);
        vx(j, i) = t;
        }

    fclose(fp);

    return &vx;
}


void pgmwrite (char *filename, Matrix<float>& img, float thresh)
{
    FILE *fp;
    int i, j, k;

    if (NULL == (fp = fopen(filename, "w")))
        {
	fprintf(stderr, "pgmwrite: cannot create <%s>\n", filename);
	exit(-1);
        }

    int nx = img.cols();
    int ny = img.rows();
    fprintf(fp, "P2\n");
    fprintf(fp, "# Written by pgmwrite\n");
    fprintf(fp, "%d %d\n", nx, ny);
    fprintf(fp, "%d\n", (int)thresh);

    k = 0;

    for (j = 0; j < ny; j++)
        for (i = 0; i < nx; i++) {
	        fprintf(fp, "%3d ", (int)img(j, i));
	        if (0 == (k+1)%16) fprintf(fp, "\n");
	        k++;
        }
   
    if (0 != k%16) fprintf(fp, "\n");
    fclose(fp);
}
