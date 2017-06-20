#include "MPIGLProcess.h"
#include <stdio.h>
#include "Commons.h"
#include "../tools/Matrix.h"

void MPIGLProcess::initialize()
{
    MPI_Comm_rank(mCommunicator, &mRank);
    MPI_Cart_shift(mCommunicator, 0, 1, &mLeftProc, &mRightProc);
    MPI_Cart_shift(mCommunicator, 1, 1, &mUpProc, &mDownProc);

    int dimsCnt;
    MPI_Cartdim_get(mCommunicator, &dimsCnt);
    int* coords = new int[dimsCnt];
    int* dims = new int[dimsCnt];
    int* periods = new int[dimsCnt];
    MPI_Cart_get(mCommunicator, dimsCnt, dims, periods, coords);

    printf("P%d: %dx%d\n", mRank, coords[1], coords[0]);
    printf("P%d:\tl: %d\tr: %d\tu: %d\td: %d\n", mRank, mLeftProc, mRightProc, mUpProc, mDownProc);
    if (!mRank) {
        printf("Cart %dx%d\n", dims[1], dims[0]);
    }

    int width, height;
    parseWH(mInputFile, width, height);
    if (!mRank) {
        printf("Input %dx%d\n", width, height);
    }

    int localWidth, widthOffset;
    computeProcessWork(dims[0], coords[0], width, localWidth, widthOffset);
    int localHeight, heightOffset;
    computeProcessWork(dims[1], coords[1], height, localHeight, heightOffset);

    printf("P%d: Local data - %dx%d\n", mRank, localWidth, localHeight);
    printf("P%d: Start point - %dx%d\n", mRank, widthOffset, heightOffset);


    MPI_Datatype fileView;
    MPI_Type_vector(localHeight, localWidth, width + 1, MPI_CHAR, &fileView);
    MPI_Type_commit(&fileView);

    MPI_File file;
    MPI_File_open(mCommunicator, &mInputFile[0], MPI_MODE_RDONLY, MPI_INFO_NULL, &file);
    int fileOffset = (heightOffset * (width + 1)) + widthOffset;
    MPI_File_set_view(file, fileOffset, MPI_CHAR, fileView, "native", MPI_INFO_NULL);

    MPI_Datatype row;
    MPI_Type_contiguous(localWidth, MPI_CHAR, &row);
    MPI_Type_commit(&row);

    Matrix<char> inputBoard(localHeight, localWidth);
    MPI_File_read_all(file, inputBoard.ptr(), localHeight, row, MPI_STATUS_IGNORE);

    Matrix<bool>& localBoard = *convertToBool(inputBoard);
    mProcess->setBoard(localBoard);


    MPI_File_close(&file);
    MPI_Type_free(&row);
    MPI_Type_free(&fileView);

    deleteObject(localBoard);
    deleteArray(dims);
    deleteArray(coords);
    deleteArray(periods);
}

void MPIGLProcess::finalize()
{
    Matrix<bool>& localBoard = *(mProcess->getBoard());
    Matrix<char>* outBoard = convertToChar(localBoard);
    int cols = outBoard->cols();

    char* firstLine = new char[cols + 1];
    copyArray(firstLine, outBoard->getRowPtr(0), cols);
    firstLine[cols] = 0;
    printf("P%d: First line: %s\n", mRank, firstLine);

    deleteArray(firstLine);
    deleteObject(outBoard);
    deleteObject(localBoard);
}
