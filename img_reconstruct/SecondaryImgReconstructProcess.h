#pragma once
#include "ImgReconstructProcess.h"
#include "../tools/ProcessDecorator.h"

class SecondaryImgReconstructProcess : public ProcessDecorator<ImgReconstructProcess>
{
public:
    SecondaryImgReconstructProcess(ImgReconstructProcess* process)
        : ProcessDecorator(process) {}
    
    virtual void initialize();

    virtual void finalize();
};

