#include "ImgReconstructProcess.h"

#define NEIHBOURHOOD_SIZE 4
const char NEIHBOURHOOD[][2] = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };

void ImgReconstructProcess::setImg(Matrix<float>& img)
{
    mOriginalImg = new Matrix<float>(img);
}

Matrix<float>* ImgReconstructProcess::getImg()
{
    return new Matrix<float>(*mProcessedImg);
}

void ImgReconstructProcess::initialize()
{
    int rows = mOriginalImg->rows();
    int cols = mOriginalImg->cols();

    mProcessedImg = new Matrix<float>(rows + 2, cols);
    memcpy(mProcessedImg->ptr() + cols, mOriginalImg->ptr(), mOriginalImg->size() * sizeof(float));

    mBuffImg = new Matrix<float>(rows + 2, cols);
    //printf("P%d:\tpixels: %d\trows: %d\tcols: %d\n",mRank, img.size(), img.rows(), img.cols());

}

void ImgReconstructProcess::syncData()
{
    //syncronizing data
    int cols = mProcessedImg->cols();
    float* beginPtr = mProcessedImg->ptr();
    float* endPtr = beginPtr + mProcessedImg->size();

    MPI_Sendrecv(endPtr - 2 * cols, cols, MPI_FLOAT, mNextProcRank, 0,
        beginPtr, cols, MPI_FLOAT, mPrevProcRank, 0,
        mCommunicator, MPI_STATUS_IGNORE);
    MPI_Sendrecv(beginPtr + cols, cols, MPI_FLOAT, mPrevProcRank, 0,
        endPtr - cols, cols, MPI_FLOAT, mNextProcRank, 0,
        mCommunicator, MPI_STATUS_IGNORE);
}

void ImgReconstructProcess::processData()
{

    for (int i = 1; i < mProcessedImg->rows(); i++) {
        for (int j = 0; j < mProcessedImg->cols(); j++)
        {
            float x = (*mOriginalImg)(i, j);
            for (int l = 0; l < NEIHBOURHOOD_SIZE; l++) {
                float ci = i + NEIHBOURHOOD[l][0];
                float cj = j + NEIHBOURHOOD[l][1];
                x += (*mProcessedImg)(ci, cj);
            }
            (*mBuffImg)(i, j) = 0.25 * x;
        }
    }

    *mProcessedImg = *mBuffImg;
}


