#include <mpi.h>
#include <stdio.h>
#include "Commons.h"
#include "WorkProcess.h"
#include "GUIProcess.h"

int main(int argc, char **argv)
{
    /* Initialize MPI environment */
    MPI_Init(&argc, &argv);
    checkARGV(argc, argv);

    /* Get communicator size */
    int processes;
    MPI_Comm_size(MPI_COMM_WORLD, &processes);
    if (processes < 2) {
        printf("At least two processes required - one for GUI and at least on for processing.\n");
        exit(1);
    }

    std::vector<int> dims(DIMS, 0);
    MPI_Dims_create(processes - 1, DIMS, &dims[0]);
    MPI_Comm communicator;
    std::vector<int> periods(DIMS, 0);
    MPI_Cart_create(MPI_COMM_WORLD, DIMS, &dims[0], &periods[0], true, &communicator);

    Process* process;
    if (communicator == MPI_COMM_NULL) {
        process = new GUIProcess();
    } else {
        process = new WorkProcess(argv, communicator);
    }

    process->initialize();
    process->processData();
    process->finalize();

    MPI_Finalize();

    deleteObject(process);

    return 0;
}