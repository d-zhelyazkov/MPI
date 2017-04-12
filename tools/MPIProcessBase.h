#pragma once
#include "Process.h"
#include <mpi.h>

class MPIProcessBase : public Process {
protected:
    MPI_Comm mCommunicator;
    int mRank;

public:
    MPIProcessBase(const MPI_Comm& communiator, int rank) : mCommunicator(communiator),
        mRank(rank) {}

    MPI_Comm* getCommunicator() {
        return &mCommunicator;
    }

    int getRank() {
        return mRank;
    }
};