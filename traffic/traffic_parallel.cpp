#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define GN 64 
#define ITERATIONS 20

#define send(byte, dest) MPI_Send(&byte, 1, MPI_BYTE, dest, 0, MPI_COMM_WORLD)
#define recv(byte, source) MPI_Recv(&byte, 1, MPI_BYTE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)

int main(int argc, char **argv)
{
    int size, rank;
    int nrank, prank;

    /* Initialize MPI environment */
    MPI_Init(&argc, &argv);

    /* Get communicator size */
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    /* Get process rank in the world communicator */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Calculate neighbouring ranks */
    nrank = (rank + 1) % size;
    prank = (rank - 1 + size) % size;

    int N = GN / size;
    if (rank < GN % size)
        N++;

    char* str = new char[N + 2];
    char* newstr = new char[N + 2];
    char buf;
    int i, j, Ncar;
    float p, x, v;
    p = 0.5;
    Ncar = 0;

    for (i = 1; i <= N; i++) {

        x = (float)rand() / (RAND_MAX);
        if (x < p) {
            str[i] = 'D';
            Ncar++;
        }
        else {
            str[i] = ' ';
        }
    }

    for (i = 1; i <= ITERATIONS; i++) {
        if (rank) {
            //wait for prev 
            recv(buf, prank);
        }
        else {
            printf("\n");
        }

        for (j = 1; j <= N; j++)
            printf("%c", str[j]);
        if (rank + 1 < size)
            send(buf, nrank);


        v = 0;
        /*str[0] = str[N];
        str[N + 1] = str[1];*/
        if (rank & 1) {
            send(str[N], nrank);
            recv(str[N + 1], nrank);

            send(str[1], prank);
            recv(str[0], prank);
        }
        else {
            recv(str[0], prank);
            send(str[1], prank);

            recv(str[N + 1], nrank);
            send(str[N], nrank);
        }


        for (j = 1; j <= N; j++) {
            if (str[j] == ' ') {
                newstr[j] = str[j - 1];
            }
            else {
                newstr[j] = str[j + 1];
            }

            if (newstr[j] != str[j])
                v += 1;

        }

        memcpy(str, newstr, N + 2);

        v = v / (2 * Ncar);
        
    }



    MPI_Finalize();

    return 0;
}