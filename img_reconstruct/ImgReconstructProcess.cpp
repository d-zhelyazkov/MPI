#include "ImgReconstructProcess.h"


void ImgReconstructProcess::initialize()
{
    int rows = mOriginalImg->rows();
    int cols = mOriginalImg->cols();

    mProcessedImg = new Matrix<float>(rows + 2, cols + 2);
    for (int i = 0; i < rows; i++) {
        float* imgRow = mOriginalImg->getRowPtr(i);
        float* processRow = &mProcessedImg->getRowPtr(i + 1)[1];
        copyArray(processRow, imgRow, cols);
    }

    mBuffImg = new Matrix<float>(rows + 2, cols + 2);
    //printf("P%d:\tpixels: %d\trows: %d\tcols: %d\n",mRank, img.size(), img.rows(), img.cols());

}

void ImgReconstructProcess::syncData()
{
    //syncronizing data
    int cols = mProcessedImg->cols();
    int rows = mProcessedImg->rows();

    MPI_Sendrecv(mProcessedImg->getRowPtr(rows - 2), cols, MPI_FLOAT, mNextProcRank, 0,
        mProcessedImg->getRowPtr(0), cols, MPI_FLOAT, mPrevProcRank, 0,
        mCommunicator, MPI_STATUS_IGNORE);
    MPI_Sendrecv(mProcessedImg->getRowPtr(1), cols, MPI_FLOAT, mPrevProcRank, 0,
        mProcessedImg->getRowPtr(rows - 1), cols, MPI_FLOAT, mNextProcRank, 0,
        mCommunicator, MPI_STATUS_IGNORE);
}

void ImgReconstructProcess::processData()
{

    for (int i = 1; i < mProcessedImg->rows() - 1; i++) {
        for (int j = 1; j < mProcessedImg->cols() - 1; j++)
        {
            (*mBuffImg)(i, j) = 0.25 * ((*mProcessedImg)(i - 1, j) + (*mProcessedImg)(i + 1, j) +
                (*mProcessedImg)(i, j - 1) + (*mProcessedImg)(i, j + 1) + (*mOriginalImg)(i - 1, j - 1));
        }
    }

    *mProcessedImg = *mBuffImg;
}


