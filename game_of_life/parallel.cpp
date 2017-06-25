#include <mpi.h>
#include <stdio.h>
#include "Commons.h"
#include "MPIGLProcess.h"
#include "GLProcess.h"
#include "../tools/MainProcess.h"

#define DIMS 2

int main(int argc, char **argv)
{
    /* Initialize MPI environment */
    MPI_Init(&argc, &argv);
    checkARGV(argc, argv);

    string inputFile(argv[1]);
    int iterations = atoi(argv[2]);
    string& outFile = *getOutputFileName(inputFile, iterations, "parallel");

    /* Get communicator size */
    int processes;
    MPI_Comm_size(MPI_COMM_WORLD, &processes);

    vector<int> dims(DIMS, 0);
    MPI_Dims_create(processes, DIMS, &dims[0]);
    MPI_Comm communicator;
    vector<int> periods(DIMS, 0);
    MPI_Cart_create(MPI_COMM_WORLD, DIMS, &dims[0], &periods[0], true, &communicator);
    
    GLProcess glProcess;
    MPIGLProcess* mpiProcess = new MPIGLProcess(glProcess, inputFile, outFile, communicator);

    int rank;
    MPI_Comm_rank(communicator, &rank);
    Process* process;
    if (rank) {
        process = mpiProcess;
    } else {
        process = new MainProcess(*mpiProcess, ITER_NOTIFICATION);
        deleteObject(mpiProcess);
    }

    process->initialize();
    for (int i = 0; i < iterations; i++) {
        process->processData();
    }
    process->finalize();

    MPI_Finalize();

    deleteObject(process);
    deleteObject(outFile);
    return 0;
}