#include <mpi.h>
#include <stdio.h>
#include "Commons.h"
#include "../tools/Tools.h"

#define DIMS 2

void computeProcessesWork(int processes, int work, int*& sizes, int*& offsets);

int main(int argc, char **argv)
{
    /* Initialize MPI environment */
    MPI_Init(&argc, &argv);
    checkARGV(argc, argv);

    /* Get communicator size */
    int processes;
    MPI_Comm_size(MPI_COMM_WORLD, &processes);

    int dims[DIMS] = { 0 };
    MPI_Dims_create(processes, DIMS, dims);
    MPI_Comm communicator;
    int periods[DIMS] = { 0 };
    MPI_Cart_create(MPI_COMM_WORLD, DIMS, dims, periods, true, &communicator);
    
    int rank, left, right, up, down;
    MPI_Comm_rank(communicator, &rank);
    MPI_Cart_shift(communicator, 0, 1, &left, &right);
    MPI_Cart_shift(communicator, 1, 1, &up, &down);

    if (!rank) {
        printf("Cart %dx%d\n", dims[0], dims[1]);
    }
    int coords[DIMS];
    MPI_Cart_coords(communicator, rank, DIMS, coords);
    printf("P%d: %dx%d\n", rank, coords[0], coords[1]);
    printf("P%d:\tl: %d\tr: %d\tu: %d\td: %d\n", rank, left, right, up, down);
    
    

    int width, height;
    parseWH(string(argv[1]), width, height);
    if (!rank) {
        printf("Input %dx%d\n", width, height);
    }
    int* localWidths, *widthsOffsets;
    computeProcessesWork(dims[0], width, localWidths, widthsOffsets);
    int localWidth = localWidths[coords[0]];
    int widthOffset = widthsOffsets[coords[0]];
    deleteArray(localWidths);
    deleteArray(widthsOffsets);

    int* localHeights, *heightsOffsets;
    computeProcessesWork(dims[1], height, localHeights, heightsOffsets);
    int localHeight = localHeights[coords[1]];
    int heightOffset = heightsOffsets[coords[1]];
    deleteArray(localHeights);
    deleteArray(heightsOffsets);
    
    printf("P%d: Local data - %dx%d\n", rank, localWidth, localHeight);
    printf("P%d: Start point - %dx%d\n", rank , widthOffset, heightOffset);



    /*MPI_File file;
    MPI_File_open(communicator, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
    MPI_File_read*/

    MPI_Finalize();

    return 0;
}

void computeProcessesWork(int processes, int work, int*& sizes, int*& offsets) {
    sizes = new int[processes];
    offsets = new int[processes];
    int currOffset = 0;
    int reminders = work % processes;
    int N = work / processes;
    for (int i = 0; i < processes; i++) {
        int size = (i < reminders) ? N + 1 : N;
        sizes[i] = size;
        offsets[i] = currOffset;
        //printf("%d:\twork: %d\toffset: %d\n", i, size, currOffset);
        currOffset += size;
    }
}