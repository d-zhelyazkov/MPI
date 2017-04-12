#pragma once
#include <mpi.h>
#include "Matrix.h"

#define MAIN_PROC 0
#define THRESH 255

class ImgReconstructProcess
{
private:
    int mNextProcRank, mPrevProcRank;
    Matrix<float>* mBuffImg = NULL;

protected:
    MPI_Comm mCommunicator;
    int mRank;
    Matrix<float>* mOriginalImg = NULL;
    Matrix<float>* mProcessedImg = NULL;

public:
    ImgReconstructProcess(const MPI_Comm& communiator, int rank, int prevProcRank, int nextProcRank) :
    mCommunicator(communiator),
        mRank(rank),
        mPrevProcRank(prevProcRank),
        mNextProcRank(nextProcRank) {}

    ~ImgReconstructProcess() {
        void* pointers[] = { mOriginalImg , mProcessedImg, mBuffImg};
        for (void* ptr : pointers) {
            if (ptr)
                delete ptr;
        }
    }

    void setImg(Matrix<float>& img);

    virtual void syncData();

    virtual void processData();

    virtual void finalize() = 0;
};

