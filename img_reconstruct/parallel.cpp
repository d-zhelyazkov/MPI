#include "MainImgReconstructProcess.h"
#include "SecondaryImgReconstructProcess.h"
#include <string.h>
#include "Commons.h"

int main(int argc, char **argv)
{
    /* Initialize MPI environment */
    MPI_Init(&argc, &argv);
    checkARGV(argc, argv);

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
    int iterations = atoi(argv[2]);
    if (rank == MAIN_PROC)
        process = new MainImgReconstructProcess(processBase, argv[1], getOutputFileName(argv[1], iterations, "parallel"));
    else
        process = new SecondaryImgReconstructProcess(processBase);
    
    //iteration loop
    process->initialize();
    clock_t processBeginTime = clock();
    double avgItTime = 0;
    for (int i = 1; i <= iterations; i++) {
        clock_t itStartTime = clock();
        process->syncData();
        process->processData();

        clock_t itTime = clock() - itStartTime;
        avgItTime += clockToS(itTime) / iterations;
        if (!rank && !(i % 1000))
            printf("%d iterations done\n", i);
    }
    if (!rank) {
        printf("Processing completed - %fs\n", clockToS(clock() - processBeginTime));
        printf("Avg iteration time: %fs\n", avgItTime);
    }

    process->finalize();

    MPI_Finalize();

    deleteObject(processBase);
    deleteObject(process);

    return 0;
}