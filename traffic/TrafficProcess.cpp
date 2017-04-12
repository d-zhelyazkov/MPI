#include "TrafficProcess.h"

void TrafficProcess::syncData()
{
    //syncronizing data
    MPI_Request request = MPI_REQUEST_NULL;
    MPI_Ibsend(&mStreet[1], 1, MPI_CHAR, mPrevProcRank, 0, mCommunicator, &request);
    MPI_Ibsend(&mStreet[mStreet.size() - 2], 1, MPI_CHAR, mNextProcRank, MAIN_PROC, mCommunicator, &request);
    MPI_Recv(&mStreet[0], 1, MPI_CHAR, mPrevProcRank, MAIN_PROC, mCommunicator, MPI_STATUS_IGNORE);
    MPI_Recv(&mStreet[0], 1, MPI_CHAR, mPrevProcRank, MAIN_PROC, mCommunicator, MPI_STATUS_IGNORE);

    MPI_Sendrecv(&mStreet[mStreet.size() - 2], 1, MPI_CHAR, mNextProcRank,0,
        &mStreet[0], 1, MPI_CHAR, mPrevProcRank, MAIN_PROC,
        mCommunicator, MPI_STATUS_IGNORE)

}

void TrafficProcess::processData() {
    std::vector<char> newStreet(mStreet.size());

    //process data
    for (int j = 1; j < mStreet.size() + 1; j++) {
        newStreet[j] = (mStreet[j] == SPACE) ? mStreet[j - 1] : mStreet[j + 1];
    }
    mStreet = newStreet;
}


