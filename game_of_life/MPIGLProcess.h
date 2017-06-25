#pragma once
#include "../tools/ProcessDecorator.h"
#include "GLProcess.h"
#include <string>
#include <mpi.h>

using std::string;

class MPIGLProcess :
    public ProcessDecorator
{
private:
    string mInputFile;
    string mOutputFile;
    GLProcess* mProcess;
    MPI_Comm mCommunicator;

    int mRank, mLeftProc, mRightProc, mUpProc, mDownProc;

    int mFileOffset;
    bool mLastRowProcess;

    MPI_Datatype mColumnType;

public:
    MPIGLProcess(GLProcess& process, string& inputFile, string& outputFile, MPI_Comm& communicator) :
        ProcessDecorator(process),
        mInputFile(inputFile),
        mOutputFile(outputFile),
        mCommunicator(communicator) {

        mProcess = (GLProcess*)mBaseProcess;
    }
    
    virtual void initialize();

    virtual void finalize();

    void syncData();

    virtual void processData() {
        ProcessDecorator::processData();

        syncData();
    }

    virtual MPIGLProcess* clone() {
        return new MPIGLProcess(*mProcess, mInputFile, mOutputFile, mCommunicator);
    }

private:
    void setColumnType();
};

