#include "WorkProcess.h"
#include "Commons.h"
#include "MPIGLProcess.h"
#include "GLProcess.h"
#include "../tools/MainProcess.h"
#include "../tools/MPITimeProvider.h"

void WorkProcess::initialize() {

    string inputFile(ARGV[1]);
    mIterations = atoi(ARGV[2]);
    string& outFile = *getOutputFileName(inputFile, mIterations, "parallel");

    int rank;
    MPI_Comm_rank(mCommunicator, &rank);
    printf("P%d: Work process created.\n", rank);

    GLProcess glProcess;
    MPIGLProcess* mpiProcess = 
        new MPIGLProcess(glProcess, inputFile, outFile, mCommunicator);

    if (rank) {
        mProcess = mpiProcess;
    }
    else {
        //the zero ranked process is the reporting one
        MPITimeProvider timeProvider;
        mProcess = new MainProcess(*mpiProcess, ITER_NOTIFICATION, &timeProvider);
        deleteObject(mpiProcess);
    }

    mProcess->initialize();

    deleteObject(outFile);
}

void WorkProcess::processData()
{
    for (int i = 0; i < mIterations; i++) {
        mProcess->processData();
    }
}

void WorkProcess::finalize()
{
    mProcess->finalize();

    deleteObject(mProcess);
}
