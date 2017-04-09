#pragma once
#include "TrafficProcess.h"


class SlaveTrafficProcess : public TrafficProcess
{
public:
    SlaveTrafficProcess(const MPI_Comm& communiator, int rank, int prevProcRank, int nextProcRank);
    
    virtual void processData();

    void sendStreetToMain();

    virtual void finalize();
};

