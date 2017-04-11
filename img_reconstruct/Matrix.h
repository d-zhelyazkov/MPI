#pragma once
#include <string.h>
#include <stdlib.h>
#include <vector>

using std::vector;

template <typename T>
void templateCalloc(T*& arrPtr, unsigned size) {
    arrPtr = (T*)calloc(size, sizeof(T));
}

template <typename T>
class Matrix
{
private:
    vector<T> mArray;
    unsigned mRows, mCols;

public:
    Matrix(unsigned rows, unsigned cols) : mRows(rows), mCols(cols) {
        mArray.resize(mRows * mCols);
    }

    Matrix(Matrix<T>& matrix) {
        *this = matrix;
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
        mRows = matrix.mRows;
        mCols = matrix.mCols;
        mArray = matrix.mArray;

        return *this;
    }

    T* ptr() {
        return &mArray[0];
    }

    unsigned rows() {
        return mRows;
    }

    unsigned cols() {
        return mCols;
    }

    unsigned size() {
        return mArray.size();
    }

private:
    bool isInRange(unsigned row, unsigned col) {
        return (row < mRows && col < mCols);
    }
};

