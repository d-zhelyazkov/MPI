#include "ImgReconstructProcessFactory.h"

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

    ImgReconstructProcess* process = ImgReconstructProcessFactory::construct(communicator);

    int rank;
    /* Get process rank in the communicator */
    MPI_Comm_rank(communicator, &rank);

    //iteration loop
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

    delete process;

    return 0;
}