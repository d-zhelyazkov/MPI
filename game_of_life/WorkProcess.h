#pragma once
#include "../tools/Process.h"
#include <mpi.h>

class WorkProcess :
    public Process
{
private:
    char** ARGV;
    MPI_Comm mCommunicator;
    int mIterations;
    Process* mProcess;

public:
    WorkProcess(char* argv[], MPI_Comm& comm) :
        ARGV(argv),
        mCommunicator(comm) {}

    void initialize();

    void processData();

    void finalize();

    WorkProcess* clone() {
        return new WorkProcess(ARGV, mCommunicator);
    }
};

