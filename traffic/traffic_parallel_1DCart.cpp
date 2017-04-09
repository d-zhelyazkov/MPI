#include "TrafficProcessFactory.h" 

#define ITERATIONS 20

int main(int argc, char **argv)
{
    /* Initialize MPI environment */
    MPI_Init(&argc, &argv);

    /* Get communicator size */
    int processes;
    MPI_Comm_size(MPI_COMM_WORLD, &processes);

    int dims[] = { processes };
    int periods[] = { true };
    MPI_Comm communicator;
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, true, &communicator);

    TrafficProcess* process = TrafficProcessFactory::getProccess(communicator);

    //iteration loop
    for (int i = 1; i <= ITERATIONS; i++) {
        process->syncData();
        process->processData();
    }
    process->finalize();

    MPI_Finalize();

    delete process;

    return 0;
}
