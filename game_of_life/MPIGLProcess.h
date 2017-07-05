#pragma once
#include "../tools/ProcessDecorator.h"
#include "GLProcess.h"
#include <string>
#include <mpi.h>

using std::string;

/*
* MPI Game of Life process. Implements the following functionalities:
*   - sync data with other MPI GoL processes.
*   - reads parallely the local pattern from the input file
*   - writes parallely the local pattern in the output file
*   - handles GUIProcess's requests asynchronously
*/
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
    MPI_Datatype mMatrixType;

    int mGuiProc;
    MPI_Request mGuiRequest;
    bool mGuiRequestBuf;
    MPI_Request mGuiSendRequest;

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

        syncData();

        ProcessDecorator::processData();
    }

    virtual MPIGLProcess* clone() {
        return new MPIGLProcess(*mProcess, mInputFile, mOutputFile, mCommunicator);
    }

private:
    void setMPIDatatypes();
    void setGuiRequestWait();
};

