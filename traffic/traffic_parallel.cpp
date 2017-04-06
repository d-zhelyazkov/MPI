#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define STREET_SIZE 64 
#define ITERATIONS 20
#define P 0.5
#define CAR 'D'
#define SPACE ' '

char* STREET;
int N;
int THREADS, rank;

#define send(byte, dest) MPI_Send(&byte, 1, MPI_BYTE, dest, 0, MPI_COMM_WORLD)
#define recv(byte, source) MPI_Recv(&byte, 1, MPI_BYTE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
//#define sendStreet() MPI_Send(&STREET[1], N, MPI_CHAR, 0, 0, MPI_COMM_WORLD)


char* syncWholeStreet();

int main(int argc, char **argv)
{
    /* Initialize MPI environment */
    MPI_Init(&argc, &argv);

    /* Get communicator size */
    MPI_Comm_size(MPI_COMM_WORLD, &THREADS);
    /* Get process rank in the world communicator */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* Calculate neighboring ranks */
    int nrank = (rank + 1) % THREADS;
    int prank = (rank - 1 + THREADS) % THREADS;

    //Calculating current street size
    N = STREET_SIZE / THREADS;
    if (rank < STREET_SIZE % THREADS)
        N++;
    
    printf("P%d, N:%d\n", rank, N);

    //Populating the street
    STREET = new char[N + 2];
    for (int i = 1; i <= N; i++) {
        float x = (float)((rand() * (rank + 1)) % RAND_MAX) / (RAND_MAX);
        STREET[i] = (x < P) ? CAR : SPACE;
    }

    //iteration loop
    char* newstr = new char[N + 2];
    for (int i = 1; i <= ITERATIONS; i++) {
        //printing the whole street
        char* wholeStreet = syncWholeStreet();
        if (wholeStreet) {
            printf("\n%s\n", wholeStreet);
            delete[] wholeStreet;
        }

        //syncronizing data
        if (rank & 1) {
            send(STREET[N], nrank);
            recv(STREET[N + 1], nrank);

            send(STREET[1], prank);
            recv(STREET[0], prank);
        }
        else {
            recv(STREET[0], prank);
            send(STREET[1], prank);

            recv(STREET[N + 1], nrank);
            send(STREET[N], nrank);
        }

        //process data
        for (int j = 1; j <= N; j++) {
            newstr[j] = (STREET[j] == SPACE) ? STREET[j - 1] : STREET[j + 1];
        }
        memcpy(STREET, newstr, (N + 2) * sizeof(char));
                
    }

    //gathering statistics
    char* wholeStreet = syncWholeStreet();
    if (wholeStreet) {
        printf("\n%s\n", wholeStreet);

        int cars, v;
        for (int i = 0; i < STREET_SIZE; i++) {
            if (wholeStreet[i] != CAR)
                continue;

            cars++;
            int prevIX = (i + STREET_SIZE - 1) % STREET_SIZE;
            if (wholeStreet[prevIX] == SPACE)
                v++;
        }

        printf("\nNumber of cars = %3d   Fill ratio = %7.2f  Estimated speed = %7.2f\n",
            cars,
            (float)cars/ STREET_SIZE,
            (float)v / (2 * cars));

        delete[] wholeStreet;
    }


    MPI_Finalize();

    return 0;
}

char* syncWholeStreet() {
    
    char* wholeStreet = new char[STREET_SIZE + 1];

    int* counts = new int[THREADS];
    int* offsets = new int[THREADS];

    if (!rank) {
        int currOffset = 0;
        int reminders = STREET_SIZE % THREADS;
        for (int i = 0; i < THREADS; i++) {
            int size = (!reminders || i < reminders) ? N : N - 1;
            counts[i] = size;
            offsets[i] = currOffset;
            //printf("%d\tcount: %d\toffset: %d\n", i, size, currOffset);
            currOffset += size;
        }

        memcpy(wholeStreet, STREET, N * sizeof(char));
        MPI_Gatherv(MPI_IN_PLACE, N, MPI_CHAR, wholeStreet, counts, offsets, MPI_CHAR, 0, MPI_COMM_WORLD);
        wholeStreet[STREET_SIZE] = 0;
    }
    else {
        MPI_Gatherv(&STREET[1], N, MPI_CHAR, wholeStreet, counts, offsets, MPI_CHAR, 0, MPI_COMM_WORLD);

        delete[] wholeStreet;
        wholeStreet = NULL;
    }

    delete[] counts;
    delete[] offsets;

    return wholeStreet;
}