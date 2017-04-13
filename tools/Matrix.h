#pragma once
#include <string.h>
#include <stdlib.h>
#include "Tools.h"

template <typename T>
class Matrix
{
private:
    T* mArray = NULL;
    unsigned mRows = 0;
    unsigned mCols = 0;

public:
    Matrix(unsigned rows, unsigned cols) : mRows(rows), mCols(cols) {
        mArray = calloc<T>(size());
    }
    
    Matrix(Matrix<T>& matrix) {
        *this = matrix;
    }

    ~Matrix() {
        deleteArray(mArray);
    }
    
    T& operator ()(unsigned row, unsigned col)
    {
        if (!isInRange(row, col)) {
            static T dump;
            dump = 0;
            return dump;
        }

        return mArray[row * mCols + col];
    }

    Matrix<T>& operator=(Matrix<T>& matrix) {
        unsigned oldSize = size();
        mRows = matrix.mRows;
        mCols = matrix.mCols;

        unsigned newSize = matrix.size();
        if (oldSize != newSize) {
            deleteArray(mArray);
        }
        copyArray(mArray, matrix.mArray, newSize);

        return *this;
    }

    T* ptr() {
        return mArray;
    }

    unsigned rows() {
        return mRows;
    }

    unsigned cols() {
        return mCols;
    }

    unsigned size() {
        return mRows * mCols;
    }

private:
    bool isInRange(unsigned row, unsigned col) {
        return (row < mRows && col < mCols);
    }
};

template <typename T>
void print(Matrix<T>& matrix) {
    printf("Matrix:\trows: %d\tcols: \%d\tsize: %d\n", matrix.rows(), matrix.cols(), matrix.size());
}