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
MPI_Comm COMM;

#define send(byte, dest) MPI_Send(&byte, 1, MPI_BYTE, dest, 0, COMM)
#define recv(byte, source) MPI_Recv(&byte, 1, MPI_BYTE, source, 0, COMM, MPI_STATUS_IGNORE)
#define sendStreet() MPI_Send(&STREET[1], N, MPI_CHAR, 0, 0, COMM)


char* syncWholeStreet();

int main(int argc, char **argv)
{
    /* Initialize MPI environment */
    MPI_Init(&argc, &argv);

    /* Get communicator size */
    MPI_Comm_size(MPI_COMM_WORLD, &THREADS);

    int dims[] = { THREADS };
    int periods[] = { true };
    MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, true, &COMM);

    /* Get process rank in the world communicator */
    MPI_Comm_rank(COMM, &rank);

    /* Calculate neighboring ranks */
    int prank, nrank;
    MPI_Cart_shift(COMM, 0, 1, &prank, &nrank);

    //Calculating current street size
    N = STREET_SIZE / THREADS;
    if (rank < STREET_SIZE % THREADS)
        N++;

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
        if (!rank) {
            char* wholeStreet = syncWholeStreet();
            printf("\n%s\n", wholeStreet);
            delete[] wholeStreet;
        }
        else {
            sendStreet();
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
    if (!rank) {
        char* wholeStreet = syncWholeStreet();
        printf("\n%s\n", wholeStreet);

        int cars = 0;
        int v = 0;
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
            (float)cars / STREET_SIZE,
            (float)v / (2 * cars));

        delete[] wholeStreet;
    }
    else {
        sendStreet();
    }


    MPI_Finalize();

    return 0;
}

char* syncWholeStreet() {
    char* wholeStreet = new char[STREET_SIZE + 1];
    memcpy(wholeStreet, STREET, N * sizeof(char));
    int currentIt = N;

    int reminders = STREET_SIZE % THREADS;
    for (int thread = 1; thread < THREADS; thread++) {
        int streetSize = (thread < reminders) ? N : N - 1;
        MPI_Recv(&wholeStreet[currentIt], streetSize, MPI_CHAR, thread, 0, COMM, MPI_STATUS_IGNORE);
        currentIt += streetSize;
    }

    wholeStreet[STREET_SIZE] = 0;
    return wholeStreet;
}