#pragma once
#include "MPIImgReconstructProcess.h"
#include <string>

using std::string;

class MainImgReconstructProcess : public MPIImgReconstructProcess {
private:
    int mWholeImgRows;
    string mInputFile = NULL;
    string mOutputFile = NULL;

public:
    MainImgReconstructProcess(const MPI_Comm& communiator, int rank, int prevProcRank, int nextProcRank, ImgReconstructProcess& process,
        string& inputFile, string& outputFile)
        : MPIImgReconstructProcess(communiator, rank, prevProcRank, nextProcRank, process),
        mInputFile(inputFile),
        mOutputFile(outputFile) {}
    
    MainImgReconstructProcess(MainImgReconstructProcess& process) : 
        MPIImgReconstructProcess(process),
        mInputFile(process.mInputFile),
        mOutputFile(process.mOutputFile) {}

    virtual void initialize();

    virtual void finalize();

    virtual MainImgReconstructProcess* clone() {
        return new MainImgReconstructProcess(*this);
    }
};

