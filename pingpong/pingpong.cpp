#include <stdio.h>
#include <mpi.h>

#define PACKET_SIZE 100
#define NUM_ROUNDS 500000

int main(int argc, char **argv)
{
    int packet[PACKET_SIZE];

    int rank, size;
    int nrank, prank;
    int iter;

    double time_begin, time_end;

    /* Initialize MPI environment */
    MPI_Init(&argc, &argv);

    /* Get communicator size */
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    /* Get process rank in the world communicator */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Calculate neighbouring ranks */
    nrank = (rank + 1) % size;
    prank = (rank - 1 + size) % size;

    time_begin = MPI_Wtime();

    for (iter = 0; iter < NUM_ROUNDS; iter++)
    {
        if (!rank)
        {
            /* Fire! */
            MPI_Send(packet, PACKET_SIZE, MPI_INT, nrank, 0, MPI_COMM_WORLD);
            /* Wait for the packet */
            MPI_Recv(packet, PACKET_SIZE, MPI_INT, prank, 0, MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
        }
        else
        {
            /* Wait for the packet */
            MPI_Recv(packet, PACKET_SIZE, MPI_INT, prank, 0, MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
            /* Forward it */
            MPI_Send(packet, PACKET_SIZE, MPI_INT, nrank, 0, MPI_COMM_WORLD);
        }
    }

    time_end = MPI_Wtime();

    /* Only master process outputs statistics */
    if (!rank)
    {
        time_end -= time_begin;
        printf(" Number of nodes: %9d\n", size);
        printf("\n");
        printf("     Packet size: %9d Bytes\n", sizeof(packet));
        printf("Number of rounds: %9d\n", NUM_ROUNDS);
        printf("\n");
        printf("             RTT: %9.4f ms\n",
            1000.*time_end / (float)NUM_ROUNDS);
        printf("    Mean latency: %9.4f ms\n",
            1000.*time_end / (float)(size*NUM_ROUNDS));
        printf("  Mean bandwidth: %9.4f MB/s\n",
            (float)(PACKET_SIZE*NUM_ROUNDS*size) / (2.5e+5*time_end));
    }

    /* Shutdown MPI environment */
    MPI_Finalize();

    return 0;
}
