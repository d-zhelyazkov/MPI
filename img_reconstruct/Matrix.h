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
        templateCalloc<T>(mArray, getN());
    }
    ~Matrix() {
        delete[] mArray;
    }

    T& operator ()(int row, int col)
    {
        return mArray[row * mCols + col];
    }

    Matrix<T>& operator=(Matrix<T>& matrix) {
        int oldN = getN();
        int N = matrix.getN();

        mRows = matrix.mRows;
        mCols = matrix.mCols;

        if (mArray) {
            if (oldN != N) {
                delete[] mArray;
                templateCalloc<T>(mArray, N);
            }
        }
        else {
            templateCalloc<T>(mArray, N);
        }

        memcpy(mArray, matrix.mArray, sizeof(T) * N);
        return *this;
    }

    T* operator &() {
        return mArray;
    }

    int getRows() {
        return mRows;
    }

    int getCols() {
        return mRows;
    }

    int getN() {
        return mRows * mCols;
    }

};

