#pragma once
#include <string.h>
#include <stdlib.h>

template <typename T>
void templateCalloc(T*& arrPtr, unsigned size) {
    arrPtr = (T*)calloc(size, sizeof(T));
}

template <typename T>
class Matrix
{
    T* mArray = 0;
    int mRows, mCols;

public:
    Matrix(int rows, int cols) : mRows(rows), mCols(cols) {
        templateCalloc<T>(mArray, size());
    }
    ~Matrix() {
        delete[] mArray;
    }

    T& operator ()(int row, int col)
    {
        return mArray[row * mCols + col];
    }

    Matrix<T>& operator=(Matrix<T>& matrix) {
        int oldSize = size();
        int newSize = matrix.size();

        mRows = matrix.mRows;
        mCols = matrix.mCols;

        if (mArray) {
            if (oldSize != newSize) {
                delete[] mArray;
                templateCalloc<T>(mArray, newSize);
            }
        }
        else {
            templateCalloc<T>(mArray, newSize);
        }

        memcpy(mArray, matrix.mArray, sizeof(T) * newSize);
        return *this;
    }

    T* ptr() {
        return mArray;
    }

    int rows() {
        return mRows;
    }

    int cols() {
        return mRows;
    }

    int size() {
        return mRows * mCols;
    }

};

