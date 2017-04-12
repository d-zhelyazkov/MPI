#pragma once
#include "ImgReconstructProcess.h"
#include "../tools/ProcessDecorator.h"

class MainImgReconstructProcess : public ProcessDecorator<ImgReconstructProcess>
{
private:
    int mWholeImgRows;
    char* mInputFile = NULL;
    char* mOutputFile = NULL;

public:
    MainImgReconstructProcess(ImgReconstructProcess* process, char* inputFile, char* outputFile)
        : ProcessDecorator(process),
        mInputFile(inputFile),
        mOutputFile(outputFile) {}
    
    virtual void initialize();

    virtual void finalize();
};

