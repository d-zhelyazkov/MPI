#pragma once
#include "MPIImgReconstructProcess.h"

class SecondaryImgReconstructProcess : public MPIImgReconstructProcess
{
public:
    SecondaryImgReconstructProcess(const MPI_Comm& communiator, int rank, int prevProcRank, int nextProcRank, ImgReconstructProcess& process)
        : MPIImgReconstructProcess(communiator, rank, prevProcRank, nextProcRank, process) {}
    
    SecondaryImgReconstructProcess(SecondaryImgReconstructProcess& process) :
        MPIImgReconstructProcess(process) {}

    virtual void initialize();

    virtual void finalize();

    virtual SecondaryImgReconstructProcess* clone() {
        return new SecondaryImgReconstructProcess(*this);
    }
};

