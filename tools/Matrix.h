#pragma once
#include <string.h>
#include <stdlib.h>
#include "Tools.h"

template <typename T>
class Matrix
{
private:
    T** m2DArray = NULL;
    unsigned mRows = 0;
    unsigned mCols = 0;

public:
    Matrix(unsigned rows, unsigned cols) : mRows(rows), mCols(cols) {
        init2DArray();

    }
    
    Matrix(Matrix<T>& matrix) {
        *this = matrix;
    }

    ~Matrix() {
        release2dArray();
    }
    
    T& operator ()(unsigned row, unsigned col)
    {
        if (!isInRange(row, col)) {
            static T dump;
            dump = 0;
            return dump;
        }

        return m2DArray[row][col];
    }

    Matrix<T>& operator=(Matrix<T>& matrix) {
        if ((mRows != matrix.mRows) || (mCols != matrix.mCols)) {
            mRows = matrix.mRows;
            mCols = matrix.mCols;
            
            release2dArray();
            init2DArray();
        }
        copyArray(m2DArray[0], matrix.m2DArray[0], size());


        return *this;
    }

    T* ptr() {
        return m2DArray[0];
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
    void init2DArray() {
        T* arr = calloc<T>(size());
        m2DArray = new T*[mRows];
        for (int i = 0; i < mRows; i++) {
            m2DArray[i] = arr + (i * mCols);
        }
    }

    void release2dArray() {
        if (m2DArray) {
            deleteArray(m2DArray[0]);
            deleteArray(m2DArray);
        }
    }

    bool isInRange(unsigned row, unsigned col) {
        return (row < mRows && col < mCols);
    }
};

template <typename T>
void print(Matrix<T>& matrix) {
    printf("Matrix:\trows: %d\tcols: \%d\tsize: %d\n", matrix.rows(), matrix.cols(), matrix.size());
}