#include <mpi.h>
#include <stdio.h>
#include "Commons.h"
#include "../tools/Tools.h"

#define DIMS 2

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
        printf("Cart %dx%d\n", dims[1], dims[0]);
    }
    int coords[DIMS];
    MPI_Cart_coords(communicator, rank, DIMS, coords);
    printf("P%d: %dx%d\n", rank, coords[1], coords[0]);
    printf("P%d:\tl: %d\tr: %d\tu: %d\td: %d\n", rank, left, right, up, down);
    
    

    int width, height;
    parseWH(string(argv[1]), width, height);
    if (!rank) {
        printf("Input %dx%d\n", width, height);
    }

    int localWidth, widthOffset;
    computeProcessWork(dims[0], coords[0], width, localWidth, widthOffset);
    int localHeight, heightOffset;
    computeProcessWork(dims[1], coords[1], height, localHeight, heightOffset);
    
    printf("P%d: Local data - %dx%d\n", rank, localWidth, localHeight);
    printf("P%d: Start point - %dx%d\n", rank , widthOffset, heightOffset);

    MPI_Datatype row;
    MPI_Type_contiguous(localWidth, MPI_CHAR, &row);
    MPI_Type_commit(&row);

    MPI_Datatype fileView;
    MPI_Type_vector(localHeight, localWidth, width + 1, MPI_CHAR, &fileView);
    MPI_Type_commit(&fileView);

    MPI_File file;
    MPI_File_open(communicator, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
    MPI_File_set_view(file, (heightOffset * (width + 1)) + widthOffset,
        MPI_CHAR, fileView, "native", MPI_INFO_NULL);

    char* input = new char[localWidth + 1];
    MPI_File_read_all(file, input, 1, row, MPI_STATUS_IGNORE);
    input[localWidth] = 0;
    printf("P%d Line: '%s'\n", rank, input);

    deleteArray(input);
    MPI_File_close(&file);
    MPI_Type_free(&row);
    MPI_Type_free(&fileView);
    MPI_Finalize();

    return 0;
}