#pragma once
#include "../tools/Process.h"
#include "../tools/Matrix.h"


class GLProcess :
    public Process
{
private:
    Matrix<char>* mBoardBuff = NULL;
    Matrix<char>* mBoard = NULL;

public:
    ~GLProcess() {
        deleteObject(mBoard);
        deleteObject(mBoardBuff);
    }

    void setBoard(Matrix<char>& board);

    virtual void processData();

    virtual void initialize() {}

    virtual void finalize() {}

    virtual GLProcess* clone() {
        return new GLProcess();
    }

    Matrix<char>* getBoard();

    Matrix<char>* getBoardPtr() {
        return mBoard;
    }
};

