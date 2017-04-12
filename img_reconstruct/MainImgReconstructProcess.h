#pragma once
#include "ImgReconstructProcess.h"

#define INPUT_FILE "edge1024x768.dat"
#define OUTPUT_FILE "image.pgm"

class MainImgReconstructProcess : public ImgReconstructProcess
{
private:
    int mWholeImgRows;

public:
    MainImgReconstructProcess(const MPI_Comm& communiator, int rank, int prevProcRank, int nextProcRank);
    
    virtual void finalize();
};

