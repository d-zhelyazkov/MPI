#include "MainImgReconstructProcess.h"
#include "SecondaryImgReconstructProcess.h"

#define ITERATIONS 5000

int main(int argc, char **argv)
{
    /* Initialize MPI environment */
    MPI_Init(&argc, &argv);

    /* Get communicator size */
    int processes;
    MPI_Comm_size(MPI_COMM_WORLD, &processes);

    int dims[] = { processes };
    int periods[] = { false };
    MPI_Comm communicator;
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, true, &communicator);

    int rank, prevRank, nextRank;
    /* Get process rank in the communicator */
    MPI_Comm_rank(communicator, &rank);
    MPI_Cart_shift(communicator, 0, 1, &prevRank, &nextRank);
    ImgReconstructProcess* processBase = new ImgReconstructProcess(communicator, rank, prevRank, nextRank);

    Process* process;
    if (rank == MAIN_PROC)
        process = new MainImgReconstructProcess(processBase, argv[1], argv[2]);
    else
        process = new SecondaryImgReconstructProcess(processBase);
    
    //iteration loop
    process->initialize();
    double startTime = MPI_Wtime();
    for (int i = 1; i <= ITERATIONS; i++) {
        process->syncData();
        process->processData();

        if (!rank && !(i % 1000))
            printf("%d iterations done\n", i);
    }
    double endTime = MPI_Wtime();
    if(!rank)
        printf("\nProcessing time: %f\n", endTime - startTime);

    process->finalize();

    MPI_Finalize();

    deleteObject(processBase);
    deleteObject(process);

    return 0;
}