#pragma once
#include "ProcessDecorator.h"
#include <mpi.h>

class MPIProcessBase : public ProcessDecorator {
protected:
    MPI_Comm mCommunicator;
    int mRank;

public:
    MPIProcessBase(const MPI_Comm& communiator, int rank, Process& process) : 
        ProcessDecorator(process),
        mCommunicator(communiator), mRank(rank) {}
    
    MPIProcessBase(MPIProcessBase& mpiProcess) :
        mCommunicator(mpiProcess.mCommunicator), mRank(mpiProcess.mRank),
        ProcessDecorator(mpiProcess){}

    int getRank() {
        return mRank;
    }

    virtual void processData() {
        ProcessDecorator::processData();
        syncData();
    }

    virtual void syncData() = 0;
};