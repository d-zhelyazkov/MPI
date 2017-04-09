#pragma once
#include <mpi.h>
#include <vector>

#define CAR 'D'
#define SPACE ' '
#define MAIN_PROC 0


class TrafficProcess
{
private:
    int mNextProcRank, mPrevProcRank;

protected:
    MPI_Comm mCommunicator;
    std::vector<char> mStreet;
    int mRank;

public:
    TrafficProcess(const MPI_Comm& communiator, int rank, int prevProcRank, int nextProcRank) :
        mCommunicator(MPI_Comm(communiator)) ,
        mRank(rank),
        mPrevProcRank(prevProcRank),
        mNextProcRank(nextProcRank) {}
    
    virtual void syncData();

    virtual void processData();

    virtual void finalize() = 0;
    
};

