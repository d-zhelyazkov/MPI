#pragma once
#include <string.h>

#define deleteArray(arr) if(arr != NULL) {delete[] arr; arr = NULL;}
#define deleteObject(obj) if(obj != NULL) {delete obj; obj = NULL;}

template <typename T>
void calloc(T*& arr, unsigned size) {
    deleteArray(arr);
    arr = (T*)calloc(size, sizeof(T));
}

template<typename T>
T* cloneArray(T* arr, unsigned size) {
    deleteArray(arr);
    T* clone = new T[size];
    memcpy(clone, arr, size * sizeof(T));
    return clone;
}

template<typename T>
void copyArray(T*& dest, T* src, unsigned size) {
    if (dest == NULL) {
        dest = cloneArray(src, size);
    }
    else {
        memcpy(dest, src, size * sizeof(T));
    }
}

/*
*  Find the max and min absolute values of the array
*/
float arrayAbsMin(float* arr, int N);

float arrayAbsMax(float* arr, int N);

void encahnceImg(float* img, int size, float min, float max, float thresh);


