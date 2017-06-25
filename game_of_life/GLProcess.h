#pragma once
#include "../tools/Process.h"
#include "../tools/Matrix.h"


class GLProcess :
    public Process
{
private:
    Matrix<bool>* mBoardBuff = NULL;
    Matrix<bool>* mBoard = NULL;

public:
    ~GLProcess() {
        deleteObject(mBoard);
        deleteObject(mBoardBuff);
    }

    void setBoard(Matrix<bool>& board);

    virtual void processData();

    virtual void initialize() {}

    virtual void finalize() {}

    virtual GLProcess* clone() {
        return new GLProcess();
    }

    Matrix<bool>* getBoard();

    Matrix<bool>* getBoardPtr() {
        return mBoard;
    }
};

