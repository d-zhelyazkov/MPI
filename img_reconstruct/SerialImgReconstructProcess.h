#pragma once
#include "../tools/ProcessDecorator.h"
#include "ImgReconstructProcess.h"
#include <string>

using std::string;

class SerialImgReconstructProcess :
    public ProcessDecorator
{
    string mInputFile = NULL;
    string mOutputFile = NULL;
    ImgReconstructProcess* mProcess;
public:
    SerialImgReconstructProcess(ImgReconstructProcess& process, string& inputFile, string& outputFile)
        : ProcessDecorator(process),
        mInputFile(inputFile),
        mOutputFile(outputFile) {
    
        mProcess = (ImgReconstructProcess*)mBaseProcess;
    }

    SerialImgReconstructProcess(SerialImgReconstructProcess& process)
        : ProcessDecorator(process),
        mInputFile(process.mInputFile),
        mOutputFile(process.mOutputFile) {

        mProcess = (ImgReconstructProcess*)mBaseProcess;
    }

    virtual void initialize();

    virtual void finalize();

    virtual SerialImgReconstructProcess* clone() {
        return new SerialImgReconstructProcess(*mProcess, mInputFile, mOutputFile);
    }

};

