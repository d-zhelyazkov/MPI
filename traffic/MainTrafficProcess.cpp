#include "MainTrafficProcess.h"
#include <stdio.h>
#include <stdlib.h>

MainTrafficProcess::MainTrafficProcess(const MPI_Comm & communiator, int rank, int prevProcRank, int nextProcRank)
    : TrafficProcess(communiator, rank, prevProcRank, nextProcRank) {

    mWholeStreet.resize(STREET_SIZE);

    //Populating the street
    for (int i = 0; i < STREET_SIZE; i++) {
        float x = (float)rand() / (RAND_MAX);;
        mWholeStreet[i] = (x < P) ? CAR : SPACE;
    }
    //printf("MAIN: wholse street generated - %s\n", &mWholeStreet[0]);

    /* Get communicator size */
    int processes;
    MPI_Comm_size(mCommunicator, &processes);
    //Compute processes parts from the whole street
    mProcessesPartsSize.resize(processes);
    mProcessesPartsOffsets.resize(processes);
    int currOffset = 0;
    int reminders = STREET_SIZE % processes;
    int N = STREET_SIZE / processes;
    for (int i = 0; i < processes; i++) {
        int size = (i < reminders) ? N + 1: N;
        mProcessesPartsSize[i] = size;
        mProcessesPartsOffsets[i] = currOffset;
        //printf("%d\tcount: %d\toffset: %d\n", i, size, currOffset);
        currOffset += size;
    }

    mStreet.resize(mProcessesPartsSize[MAIN_PROC] + 2);
    std::copy(mWholeStreet.begin(), mWholeStreet.begin() + mProcessesPartsSize[MAIN_PROC], mStreet.begin() + 1);
    //printf("MAIN: local street: %s\n", &mStreet[1]);

    MPI_Scatter(&mProcessesPartsSize[0], 1, MPI_INT,
        MPI_IN_PLACE, 0, MPI_INT, MAIN_PROC, mCommunicator);
    //printf("MAIN: processes sizes scattered.\n");
    MPI_Scatterv(&mWholeStreet[0], &mProcessesPartsSize[0], &mProcessesPartsOffsets[0], MPI_CHAR,
        MPI_IN_PLACE, 0, MPI_CHAR, MAIN_PROC, mCommunicator);
    //printf("MAIN: processes local streets scattered.\n");
}

void MainTrafficProcess::processData()
{
    TrafficProcess::processData();
    
    syncWholeStreet();
    printf("\n%s\n", &mWholeStreet[0]);
}

void MainTrafficProcess::syncWholeStreet()
{
    std::copy(mStreet.begin() + 1, mStreet.end() - 1, mWholeStreet.begin());
    //printf("MAIN: will gather whole street.\n");
    
    MPI_Gatherv(MPI_IN_PLACE, 0, MPI_CHAR,
        &mWholeStreet[0], &mProcessesPartsSize[0], &mProcessesPartsOffsets[0], MPI_CHAR,
        MAIN_PROC, mCommunicator);
    //printf("MAIN: whole street gathered.\n");
}

void MainTrafficProcess::finalize()
{
    syncWholeStreet();
    printf("\n%s\n", &mWholeStreet[0]);

    //gathering statistics
    int cars, v;
    for (int i = 0; i < STREET_SIZE; i++) {
        if (mWholeStreet[i] != CAR)
            continue;

        cars++;
        int prevIX = (i + STREET_SIZE - 1) % STREET_SIZE;
        if (mWholeStreet[prevIX] == SPACE)
            v++;
    }

    printf("\nNumber of cars = %3d   Fill ratio = %7.2f  Estimated speed = %7.2f\n",
        cars,
        (float)cars / STREET_SIZE,
        (float)v / (2 * cars));

        
}
