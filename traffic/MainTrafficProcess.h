#pragma once
#include "TrafficProcess.h"

#define STREET_SIZE 64
#define P 0.5

class MainTrafficProcess : public TrafficProcess
{
    std::vector<int> mProcessesPartsSize;
    std::vector<int> mProcessesPartsOffsets;
    std::vector<char> mWholeStreet;

public:
    MainTrafficProcess(const MPI_Comm& communiator, int rank, int prevProcRank, int nextProcRank);

    ~MainTrafficProcess() {}

    void processData();

    void syncWholeStreet();

    virtual void finalize();

    
};

