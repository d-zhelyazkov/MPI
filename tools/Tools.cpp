#include "Tools.h"

void computeProcessWork(int processes, int rank, int globalWork,
    int & localWork, int & workOffset)
{
    localWork = globalWork / processes;
    workOffset = rank * localWork;

    int reminder = globalWork % processes;
    if (reminder) {
        if (rank < reminder) {
            localWork++;
            workOffset += rank;
        }
        else {
            workOffset += reminder;
        }
    }
}
