#pragma once
#include "../tools/Process.h"
#include <mpi.h>

/*
* MPI Process that initializes MPIGLProcess 
*   and executes iterations over it.
* Executed only by a worker process.
*/
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

