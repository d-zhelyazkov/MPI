#pragma once
#include <mpi.h>
#include "Matrix.h"


class ImgReconstructProcess
{
private:
    int mNextProcRank, mPrevProcRank;

protected:
    MPI_Comm mCommunicator;
    int mRank;
    Matrix<float>* mOriginalImg = NULL;
    Matrix<float>* mCurrentImg = NULL;
    Matrix<float>* mNewImg = NULL;

public:
    ImgReconstructProcess(const MPI_Comm& communiator, int rank, int prevProcRank, int nextProcRank) :
    mCommunicator(communiator),
        mRank(rank),
        mPrevProcRank(prevProcRank),
        mNextProcRank(nextProcRank) {}

    ~ImgReconstructProcess() {
        void* pointers[] = { mOriginalImg , mCurrentImg, mNewImg};
        for (void* ptr : pointers) {
            if (ptr)
                delete ptr;
        }
    }

    void setImg(Matrix<float>* img);

    virtual void syncData();

    virtual void processData();

    virtual void finalize() = 0;
};

