#include "SecondaryImgReconstructProcess.h"
#include "Tools.h"


SecondaryImgReconstructProcess::SecondaryImgReconstructProcess(const MPI_Comm & communiator, int rank, int prevProcRank, int nextProcRank)
    : ImgReconstructProcess(communiator, rank, prevProcRank, nextProcRank)
{
    int pixels;
    MPI_Scatter(NULL, 0, MPI_INT,
        &pixels, 1, MPI_INT, MAIN_PROC, mCommunicator);
    int cols;
    MPI_Bcast(&cols, 1, MPI_INT, MAIN_PROC, mCommunicator);

    Matrix<float> localImg(pixels / cols, cols);
    MPI_Scatterv(NULL, NULL, NULL, MPI_FLOAT,
        localImg.ptr(), localImg.size(), MPI_FLOAT, MAIN_PROC, mCommunicator);

    setImg(localImg);
}

void SecondaryImgReconstructProcess::finalize()
{
    int cols = mOriginalImg->cols();
    float* localImg = mProcessedImg->ptr() + cols;
    int localImgSize = (mProcessedImg->size() - 2 * cols);

    //compute and send local min, wait for global min
    float localMin = arrayAbsMin(localImg, localImgSize);
    MPI_Gather(&localMin, 1, MPI_FLOAT, NULL, 0, MPI_FLOAT, MAIN_PROC, mCommunicator);
    float globalMin;
    MPI_Bcast(&globalMin, 1, MPI_FLOAT, MAIN_PROC, mCommunicator);

    //compute and send local max, wait for global max
    float localMax = arrayAbsMax(localImg, localImgSize);
    MPI_Gather(&localMax, 1, MPI_FLOAT, NULL, 0, MPI_FLOAT, MAIN_PROC, mCommunicator);
    float globalMax;
    MPI_Bcast(&globalMax, 1, MPI_FLOAT, MAIN_PROC, mCommunicator);

    encahnceImg(localImg, localImgSize, globalMin, globalMax, THRESH);

    MPI_Gatherv(localImg, localImgSize, MPI_FLOAT,
        NULL, NULL, NULL, MPI_FLOAT,
        MAIN_PROC, mCommunicator);
    //printf("P%d: local image send!\n", mRank);
}
