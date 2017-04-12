#pragma once
#include <mpi.h>
#include "../tools/Tools.h"
#include "../tools/Matrix.h"
#include "../tools/MPIProcessBase.h"

#define MAIN_PROC 0
#define THRESH 255

class ImgReconstructProcess : public MPIProcessBase
{
private:
    int mNextProcRank, mPrevProcRank;
    Matrix<float>* mBuffImg = NULL;

protected:
    Matrix<float>* mOriginalImg = NULL;
    Matrix<float>* mProcessedImg = NULL;

public:
    ImgReconstructProcess(const MPI_Comm& communiator, int rank, int prevProcRank, int nextProcRank) :
        MPIProcessBase(communiator, rank),
        mPrevProcRank(prevProcRank),
        mNextProcRank(nextProcRank) {}

    ~ImgReconstructProcess() {
        void* pointers[] = { mOriginalImg , mProcessedImg, mBuffImg};
        for (void* ptr : pointers) {
            deleteObject(ptr);
        }
    }

    void setImg(Matrix<float>& img);

    Matrix<float>* getImg();

    virtual void initialize();

    virtual void syncData();

    virtual void processData();

    virtual void finalize() {};
};

