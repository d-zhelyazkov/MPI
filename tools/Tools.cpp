#include "Tools.h"
#include <math.h>

float arrayAbsMin(float* arr, int N) {

    float min = fabs(arr[0]);

    for (int i = 1; i < N; i++)
    {
        float abs = fabs(arr[i]);
        if (abs < min) min = abs;
    }

    return min;
}

float arrayAbsMax(float* arr, int N) {

    float max = fabs(arr[0]);

    for (int i = 1; i < N; i++)
    {
        float abs = fabs(arr[i]);
        if (abs > max) max = abs;
    }

    return max;
}



void encahnceImg(float* img, int size, float min, float max, float thresh) {


    for (int i = 0; i < size; i++) {
        /*
        *  Scale the value appropriately so it lies between 0 and thresh
        */
        if (min < 0 || max > thresh)
            img[i] = (int)((thresh*((fabs(img[i] - min)) / (max - min))) + 0.5);
        else
            img[i] = (int)(fabs(img[i]) + 0.5);

        /*
        *  Increase the contrast by boosting the lower values
        */
        img[i] = thresh * sqrt(img[i] / thresh);
    }
}