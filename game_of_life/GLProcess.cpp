#include "GLProcess.h"

void GLProcess::setBoard(Matrix<bool>& board)
{
    int rows = board.rows();
    int cols = board.cols();

    mBoard = new Matrix<bool>(rows + 2, cols + 2);
    for (int i = 0; i < rows; i++) {
        bool* boardRow = board.getRowPtr(i);
        bool* processedRow = &(mBoard->getRowPtr(i + 1)[1]);
        copyArray(processedRow, boardRow, cols);
    }

    mBoardBuff = new Matrix<bool>(rows + 2, cols + 2);
}

void GLProcess::processData()
{
    Matrix<bool>& board = *mBoard;
    Matrix<bool>& buff = *mBoardBuff;

    for (int i = 1; i < board.rows() - 1; i++) {
        for (int j = 1; j < board.cols() - 1; j++)
        {
            int neigbours = board(i - 1, j - 1) + board(i - 1, j) + board(i - 1, j + 1)
                + board(i, j - 1) + board(i, j + 1)
                + board(i + 1, j - 1) + board(i + 1, j) + board(i + 1, j + 1);
            
            buff(i, j) = (((neigbours == 2) & board(i, j)) 
                | (neigbours == 3));

        }
    }

    board = buff;
}

Matrix<bool>* GLProcess::getBoard()
{
    int rows = mBoard->rows() - 2;
    int cols = mBoard->cols() - 2;

    Matrix<bool>* result = new Matrix<bool>(rows, cols);
    for (int i = 0; i < rows; i++) {
        bool* boardRow = mBoard->getRowPtr(i + 1) + 1;
        bool* processedRow = result->getRowPtr(i);
        copyArray(processedRow, boardRow, cols);
    }

    return result;
}
