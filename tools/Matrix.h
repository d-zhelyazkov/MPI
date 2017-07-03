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
    Matrix(unsigned rows, unsigned cols) {
        reallocate(rows, cols);
    }
    
    Matrix(Matrix<T>& matrix) {
        *this = matrix;
    }

    Matrix() {}

    ~Matrix() {
        deleteArray(mArray);
    }
    
    //unsafe
    T& operator ()(unsigned row, unsigned col)
    {
        return mArray[row * mCols + col];
    }

    Matrix<T>& operator=(Matrix<T>& matrix) {
        unsigned oldSize = size();
        unsigned newSize = matrix.size();
        if (oldSize != newSize) {
            reallocate(matrix.rows(), matrix.cols());
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

    //unsafe
    T* getRowPtr(unsigned row) {
        return &mArray[row * mCols];
    }

    void reallocate(unsigned rows, unsigned cols) {
        mRows = rows;
        mCols = cols;

        deleteArray(mArray);
        mArray = calloc<T>(size());

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