#include <stdio.h>
#include <stdlib.h>

#define N 64 

int main() {
    char str[N + 2], newstr[N + 2];
    int i, j, Nit, Ncar;
    float p, x, v;

    Nit = 20;
    p = 0.5;
    Ncar = 0;
    for (i = 1; i <= N; i++) {

        x = (float)rand() / (RAND_MAX);
        if (x < p) {
            str[i] = 'D';
            Ncar++;
        }
        else {
            str[i] = ' ';
        }
    }

    printf("\n");
    for (i = 1; i <= Nit; i++) {
        v = 0;
        str[0] = str[N];
        str[N + 1] = str[1];

        for (j = 1; j <= N; j++) {
            if (str[j] == ' ') {
                newstr[j] = str[j - 1];
            }
            else {
                newstr[j] = str[j + 1];
            }

            if (newstr[j] != str[j])
                v += 1;

        }

        for (j = 1; j <= N; j++)
            str[j] = newstr[j];

        v = v / (2 * Ncar);

        for (j = 1; j <= N; j++)
            printf("%c", str[j]);
        printf("\n");

    }

    printf("\nNumber of cars = %3d   Fill ratio = %7.2f  Estimated speed = %7.2f\n", Ncar, Ncar * 1.0 / N, v);

    return 0;
}
