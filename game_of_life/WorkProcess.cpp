#include "WorkProcess.h"
#include "Commons.h"
#include "MPIGLProcess.h"
#include "GLProcess.h"
#include "../tools/MainProcess.h"

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
        mProcess = new MainProcess(*mpiProcess, ITER_NOTIFICATION);
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
