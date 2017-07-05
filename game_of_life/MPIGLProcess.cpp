#include "MPIGLProcess.h"
#include <stdio.h>
#include "Commons.h"
#include "../tools/Matrix.h"

char NATIVE[] = "native";

/*
* Appends newline char to each row of the matrix.
* Matrix col increments.
*/
void appendNewlines(Matrix<char>& matrix);

void MPIGLProcess::initialize()
{
    //retreives rank and neighbours in cartesian topology
    MPI_Comm_rank(mCommunicator, &mRank);
    MPI_Cart_shift(mCommunicator, 0, 1, &mLeftProc, &mRightProc);
    MPI_Cart_shift(mCommunicator, 1, 1, &mUpProc, &mDownProc);

    //retreives topology size and process coordinates
    int coords[DIMS];
    int dims[DIMS];
    int periods[DIMS];
    MPI_Cart_get(mCommunicator, DIMS, dims, periods, coords);

    printf("P%d: %dx%d\n", mRank, coords[1], coords[0]);
    printf("P%d:\tl: %d\tr: %d\tu: %d\td: %d\n", mRank, mLeftProc, mRightProc, mUpProc, mDownProc);
    if (!mRank) {
        printf("Cart %dx%d\n", dims[1], dims[0]);
    }
    //determines if the proccess is the last one on his row in the cartesian topology
    mLastRowProcess = (coords[0] == (dims[0] - 1));

    int width, height;
    parseWH(mInputFile, width, height);
    if (!mRank) {
        printf("Input %dx%d\n", width, height);
    }

    //compute process's local board size and offset
    int localWidth, widthOffset;
    computeProcessWork(dims[0], coords[0], width, localWidth, widthOffset);
    int localHeight, heightOffset;
    computeProcessWork(dims[1], coords[1], height, localHeight, heightOffset);
    printf("P%d: Local data - %dx%d\n", mRank, localWidth, localHeight);
    printf("P%d: Start point - %dx%d\n", mRank, heightOffset, widthOffset);

    Matrix<char> inputBoard(localHeight, localWidth);
    
    //declares process's view(board) in the input file
    MPI_Datatype fileView;
    //width + 1 because of the newline char at the end of each line
    MPI_Type_vector(localHeight, localWidth, width + 1, MPI_CHAR, 
        &fileView);
    MPI_Type_commit(&fileView);

    MPI_File file;
    MPI_File_open(mCommunicator, &mInputFile[0], MPI_MODE_RDONLY, 
        MPI_INFO_NULL, &file);

    //computes the pointer in the linear file
    mFileOffset = (heightOffset * (width + 1)) + widthOffset;
    MPI_File_set_view(file, mFileOffset, MPI_CHAR, fileView, 
        NATIVE, MPI_INFO_NULL);

    MPI_File_read(file, inputBoard.ptr(), inputBoard.size(), MPI_CHAR, 
        MPI_STATUS_IGNORE);

    MPI_File_close(&file);

    Matrix<char>& localBoard = *convertToBool(inputBoard);
    mProcess->setBoard(localBoard);

    setMPIDatatypes();

    int processes;
    MPI_Comm_size(MPI_COMM_WORLD, &processes);
    //the gui process is always the last one
    mGuiProc = processes - 1;

    //sending asynchronously the local board's size and offset
    int guiInfo[GUI_PROPS] = 
        {widthOffset, heightOffset, localWidth, localHeight};
    MPI_Isend(guiInfo, GUI_PROPS, MPI_INT,
        mGuiProc, 0, MPI_COMM_WORLD, &mGuiSendRequest);
    setGuiRequestWait();

    MPI_Type_free(&fileView);

    deleteObject(localBoard);
}

void MPIGLProcess::finalize()
{
    Matrix<char>& localBoard = *(mProcess->getBoard());
    Matrix<char>& outBoard = *convertToChar(localBoard);
    if (mLastRowProcess) {
        appendNewlines(outBoard);
    }

    int localWidth = outBoard.cols();
    int localHeight = outBoard.rows();
    printf("P%d: Preparing to write board %dx%d\n", mRank, localWidth, localHeight);
    
    int width, height;
    parseWH(mInputFile, width, height);
    
    //declares process's view(board) in the output file
    MPI_Datatype fileView;
    MPI_Type_vector(localHeight, localWidth, width + 1, MPI_CHAR, &fileView);
    MPI_Type_commit(&fileView);

    MPI_File file;
    MPI_File_open(mCommunicator, &mOutputFile[0], MPI_MODE_WRONLY | MPI_MODE_CREATE,
        MPI_INFO_NULL, &file);
    MPI_File_set_view(file, mFileOffset, MPI_CHAR, fileView,
        NATIVE, MPI_INFO_NULL);

    printf("P%d: Writing board in file from %d\n", mRank, mFileOffset);
    MPI_File_write_all(file, outBoard.ptr(), outBoard.size(), MPI_CHAR,
        MPI_STATUS_IGNORE);
    printf("P%d: Write completed.\n", mRank);

    MPI_File_close(&file);

    MPI_Type_free(&fileView);
    MPI_Type_free(&mColumnType);
    MPI_Type_free(&mMatrixType);

    deleteObject(outBoard);
    deleteObject(localBoard);
}

void MPIGLProcess::syncData()
{
    Matrix<char>& board = *(mProcess->getBoardPtr());
    int width = board.cols();
    int height = board.rows();

    MPI_Sendrecv(&board(0, width - 2), 1, mColumnType, mRightProc, 0,
        &board(0, 0), 1, mColumnType, mLeftProc, 0,
        mCommunicator, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&board(0, 1), 1, mColumnType, mLeftProc, 0,
        &board(0, width - 1), 1, mColumnType, mRightProc, 0,
        mCommunicator, MPI_STATUS_IGNORE);

    MPI_Sendrecv(&board(height - 2, 0), width, MPI_CHAR, mDownProc, 0,
        &board(0, 0), width, MPI_CHAR, mUpProc, 0,
        mCommunicator, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&board(1, 0), width, MPI_CHAR, mUpProc, 0,
        &board(height - 1, 0), width, MPI_CHAR, mDownProc, 0,
        mCommunicator, MPI_STATUS_IGNORE);

    //if data request has not been received from the GUI process
    int flag;
    MPI_Test(&mGuiRequest, &flag, MPI_STATUS_IGNORE);
    if (!flag) {
        return;
    }

    //sending asynchronously the local board
    MPI_Isend(&board(1, 1), 1, mMatrixType,
        mGuiProc, 0, MPI_COMM_WORLD, &mGuiSendRequest);

    setGuiRequestWait();
}

void MPIGLProcess::setMPIDatatypes()
{
    Matrix<char>* board = mProcess->getBoardPtr();
    int width = board->cols();
    int height = board->rows();

    //declares a column type for sending matrix columns
    MPI_Type_vector(height, 1, width,
        MPI_CHAR, &mColumnType);
    MPI_Type_commit(&mColumnType);

    //declares a matrix type for sending the whole matrix without the boundaries
    MPI_Type_vector(height - 2, width - 2, width,
        MPI_CHAR, &mMatrixType);
    MPI_Type_commit(&mMatrixType);
}

void MPIGLProcess::setGuiRequestWait()
{
    //asynchronously receive a data request from the GUI process
    MPI_Irecv(&mGuiRequestBuf, 1, MPI_C_BOOL,
        mGuiProc, 0, MPI_COMM_WORLD, &mGuiRequest);
}

void appendNewlines(Matrix<char>& matrix) {
    int rows = matrix.rows();
    int cols = matrix.cols();
    Matrix<char> result(rows, cols + 1);

    for (int i = 0; i < rows; i++) {
        char* resultLine = result.getRowPtr(i);
        char* line = matrix.getRowPtr(i);
        copyArray(resultLine, line, cols);
        resultLine[cols] = '\n';
    }

    matrix = result;
}