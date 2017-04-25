#pragma once
#include "../tools/MPIProcessBase.h"
#include "ImgReconstructProcess.h"


class MPIImgReconstructProcess :
    public MPIProcessBase
{

protected:
    int mNextProcRank, mPrevProcRank;
    ImgReconstructProcess* mProcess;

public:
    MPIImgReconstructProcess(const MPI_Comm& communiator, int rank, int prevProcRank, int nextProcRank, ImgReconstructProcess& process) :
        MPIProcessBase(communiator, rank, process),
        mPrevProcRank(prevProcRank),
        mNextProcRank(nextProcRank) {
    
        mProcess = (ImgReconstructProcess*)mBaseProcess;
    }

    MPIImgReconstructProcess(MPIImgReconstructProcess& process) :
        MPIProcessBase(process) {

        mPrevProcRank = process.mPrevProcRank;
        mNextProcRank = process.mNextProcRank;

        mProcess = (ImgReconstructProcess*)mBaseProcess;
    }


    virtual void syncData();
};

