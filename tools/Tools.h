#pragma once
#include <string.h>
#include <stdlib.h>
#include <vector>

template <typename T>
void deleteArray(T*& arr) { if (arr != NULL) { delete[] arr; arr = NULL; } }
template <typename T>
void deleteObject(T*& obj) { if (obj != NULL) { delete obj; obj = NULL; } }
template <typename T>
void deleteObject(T& obj) { delete &obj; }

template <typename T>
void deleteObjects(std::vector<T*>& objects)
{
    for (T*& obj : objects)
        deleteObject(obj);
}

template <typename T>
T* calloc(unsigned size) {
    return (T*)calloc(size, sizeof(T));
}

template<typename T>
T* cloneArray(T* arr, unsigned size) {
    T* clone = new T[size];
    memcpy(clone, arr, size * sizeof(T));
    return clone;
}

template<typename T>
void copyArray(T* dest, T* src, unsigned size) {
    memcpy(dest, src, size * sizeof(T));
}

/*
* Computes the local part from global work for a current process(rank) of several processes.
*/
void computeProcessWork(int processes, int rank, int globalWork, int& localWork, int& workOffset);