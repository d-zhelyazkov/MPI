#include "TrafficProcessFactory.h" 

#define ITERATIONS 20

int main(int argc, char **argv)
{
    /* Initialize MPI environment */
    MPI_Init(&argc, &argv);

    TrafficProcess* process = TrafficProcessFactory::getProccess(MPI_COMM_WORLD);

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
