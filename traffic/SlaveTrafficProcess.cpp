#include "SlaveTrafficProcess.h"
#include <stdio.h>


SlaveTrafficProcess::SlaveTrafficProcess(const MPI_Comm & communiator, int rank, int prevProcRank, int nextProcRank)
    : TrafficProcess(communiator, rank, prevProcRank, nextProcRank) {

    int N;
    MPI_Scatter(NULL, 0, MPI_INT,
        &N, 1, MPI_INT, MAIN_PROC, mCommunicator);
    //printf("P%d: N scattered - %d\n", rank, N);
    mStreet.resize(N + 2);
    MPI_Scatterv(NULL, NULL, NULL, MPI_CHAR,
        &mStreet[1], N, MPI_CHAR, MAIN_PROC, mCommunicator);
    //printf("P%d: local street scattered - %s\n", rank, &mStreet[1]);
}

void SlaveTrafficProcess::processData()
{
    TrafficProcess::processData();
    sendStreetToMain();
}

void SlaveTrafficProcess::sendStreetToMain()
{
    MPI_Gatherv(&mStreet[1], mStreet.size() - 2, MPI_CHAR,
        NULL, NULL, NULL, MPI_CHAR,
        MAIN_PROC, mCommunicator);
    //printf("P%d: local street send!\n", mRank);
}

void SlaveTrafficProcess::finalize()
{
    sendStreetToMain();
}
