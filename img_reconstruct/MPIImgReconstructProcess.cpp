#include "MPIImgReconstructProcess.h"


void MPIImgReconstructProcess::syncData()
{
    //syncronizing data
    Matrix<float>* processedImg = mProcess->getImgPtr();

    int cols = processedImg->cols();
    int rows = processedImg->rows();

    MPI_Sendrecv(processedImg->getRowPtr(rows - 2), cols, MPI_FLOAT, mNextProcRank, 0,
        processedImg->getRowPtr(0), cols, MPI_FLOAT, mPrevProcRank, 0,
        mCommunicator, MPI_STATUS_IGNORE);
    MPI_Sendrecv(processedImg->getRowPtr(1), cols, MPI_FLOAT, mPrevProcRank, 0,
        processedImg->getRowPtr(rows - 1), cols, MPI_FLOAT, mNextProcRank, 0,
        mCommunicator, MPI_STATUS_IGNORE);
}
