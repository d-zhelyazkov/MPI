#include "MainImgReconstructProcess.h"
#include "SecondaryImgReconstructProcess.h"
#include "../tools/MainProcess.h"
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

    Process* process;
    ImgReconstructProcess baseProcess;
    int iterations = atoi(argv[2]);
    if (rank == MAIN_PROC) {
        string inputFile(argv[1]);
        string& outputFile = *getOutputFileName(argv[1], iterations, "parallel");
        MainImgReconstructProcess rootProcess(communicator, rank, prevRank, nextRank,
            baseProcess, inputFile, outputFile);
        process = new MainProcess(rootProcess);
        delete &outputFile;
    }   
    else {
        process = new SecondaryImgReconstructProcess(communicator, rank, prevRank, nextRank, baseProcess);
    }

    process->initialize();
    //iteration loop
    for (int i = 1; i <= iterations; i++) {
        process->processData();
    }
    process->finalize();

    MPI_Finalize();

    deleteObject(process);

    return 0;
}