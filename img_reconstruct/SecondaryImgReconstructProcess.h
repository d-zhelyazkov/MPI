#pragma once
#include "ImgReconstructProcess.h"

class SecondaryImgReconstructProcess : public ImgReconstructProcess
{
public:
    SecondaryImgReconstructProcess(const MPI_Comm& communiator, int rank, int prevProcRank, int nextProcRank);
    
    virtual void finalize();
};

