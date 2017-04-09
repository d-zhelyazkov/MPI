#include "TrafficProcess.h"

void TrafficProcess::syncData()
{
    //syncronizing data
    if ((mRank % 2) == 0) {
        //is even
        MPI_Recv(&mStreet[0], 1, MPI_CHAR, mPrevProcRank, MAIN_PROC, mCommunicator, MPI_STATUS_IGNORE);
        MPI_Send(&mStreet[1], 1, MPI_CHAR, mPrevProcRank, MAIN_PROC, mCommunicator);
        //printf("P%d: successfully synced with P%d\n", mRank, mPrevProcRank);

        MPI_Recv(&mStreet[mStreet.size() - 1], 1, MPI_CHAR, mNextProcRank, MAIN_PROC, mCommunicator, MPI_STATUS_IGNORE);
        MPI_Send(&mStreet[mStreet.size() - 2], 1, MPI_CHAR, mNextProcRank, MAIN_PROC, mCommunicator);
        //printf("P%d: successfully synced with P%d\n", mRank, mNextProcRank);
    }
    else {
        MPI_Send(&mStreet[mStreet.size() - 2], 1, MPI_CHAR, mNextProcRank, MAIN_PROC, mCommunicator);
        MPI_Recv(&mStreet[mStreet.size() - 1], 1, MPI_CHAR, mNextProcRank, MAIN_PROC, mCommunicator, MPI_STATUS_IGNORE);
        //printf("P%d: successfully synced with P%d\n", mRank, mNextProcRank);

        MPI_Send(&mStreet[1], 1, MPI_CHAR, mPrevProcRank, MAIN_PROC, mCommunicator);
        MPI_Recv(&mStreet[0], 1, MPI_CHAR, mPrevProcRank, MAIN_PROC, mCommunicator, MPI_STATUS_IGNORE);
        //printf("P%d: successfully synced with P%d\n", mRank, mPrevProcRank);
    }
}

void TrafficProcess::processData() {
    std::vector<char> newStreet(mStreet.size());

    //process data
    for (int j = 1; j < mStreet.size() + 1; j++) {
        newStreet[j] = (mStreet[j] == SPACE) ? mStreet[j - 1] : mStreet[j + 1];
    }
    mStreet = newStreet;
}


