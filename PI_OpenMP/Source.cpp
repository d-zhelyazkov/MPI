#include <stdio.h>

#define ITERATIONS 1000000000

int main() {

    double pi = 0;
    double x;
    long long i;

#pragma omp parallel for private(x,i) reduction(+:pi)
    for (i = 0; i < ITERATIONS; i++) {
        x = (i % 2) ? -1 : 1;
        x /= (2 * i + 1);
        pi += x;
    }

    pi *= 4;

    printf("%f\n", pi);

    return 0;
}