#include <mpi.h>
#include <stdio.h>
#include "Commons.h"
#include "MPIGLProcess.h"
#include "GLProcess.h"

#define DIMS 2

int main(int argc, char **argv)
{
    /* Initialize MPI environment */
    MPI_Init(&argc, &argv);
    checkARGV(argc, argv);

    /* Get communicator size */
    int processes;
    MPI_Comm_size(MPI_COMM_WORLD, &processes);

    int dims[DIMS] = { 0 };
    MPI_Dims_create(processes, DIMS, dims);
    MPI_Comm communicator;
    int periods[DIMS] = { 0 };
    MPI_Cart_create(MPI_COMM_WORLD, DIMS, dims, periods, true, &communicator);
    
    string inputFile(argv[1]);
    int iterations = atoi(argv[2]);
    string& outFile = *getOutputFileName(inputFile, iterations, "parallel");
    GLProcess glProcess;
    MPIGLProcess process(glProcess, inputFile, outFile, communicator);
    process.initialize();

    process.finalize();

    MPI_Finalize();

    deleteObject(outFile);
    return 0;
}