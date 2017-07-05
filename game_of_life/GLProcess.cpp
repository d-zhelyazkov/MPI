#include "GLProcess.h"

void GLProcess::setBoard(Matrix<char>& board)
{
    int rows = board.rows();
    int cols = board.cols();

    //expands the matrix with two additional boundary rows and cols
    mBoard = new Matrix<char>(rows + 2, cols + 2);
    for (int i = 0; i < rows; i++) {
        char* boardRow = board.getRowPtr(i);
        char* processedRow = &(*mBoard)(i + 1, 1);
        copyArray(processedRow, boardRow, cols);
    }

    mBoardBuff = new Matrix<char>(rows + 2, cols + 2);
}

void GLProcess::processData()
{
    Matrix<char>& board = *mBoard;
    Matrix<char>& buff = *mBoardBuff;

    for (int i = 1; i < board.rows() - 1; i++) {
        for (int j = 1; j < board.cols() - 1; j++)
        {
            //counts the neighbours
            int neigbours = board(i - 1, j - 1) + board(i - 1, j) + board(i - 1, j + 1)
                + board(i, j - 1) + board(i, j + 1)
                + board(i + 1, j - 1) + board(i + 1, j) + board(i + 1, j + 1);
            
            //the cell is alive in case of 3 neihgbours
            //or remains as it is in case of 2
            //otherwise it is dead
            buff(i, j) = (((neigbours == 2) & board(i, j)) 
                | (neigbours == 3));

        }
    }

    board = buff;
}

Matrix<char>* GLProcess::getBoard()
{
    int rows = mBoard->rows() - 2;
    int cols = mBoard->cols() - 2;

    //constructs a non-bounded matrix
    Matrix<char>* result = new Matrix<char>(rows, cols);
    for (int i = 0; i < rows; i++) {
        char* boardRow = &(*mBoard)(i + 1, 1);
        char* processedRow = result->getRowPtr(i);
        copyArray(processedRow, boardRow, cols);
    }

    return result;
}
