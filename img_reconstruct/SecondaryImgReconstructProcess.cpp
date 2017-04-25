#include "SecondaryImgReconstructProcess.h"
#include "../tools/Tools.h"


void SecondaryImgReconstructProcess:: initialize()
{
    int pixels;
    MPI_Scatter(NULL, 0, MPI_INT,
        &pixels, 1, MPI_INT, MAIN_PROC, mCommunicator);
    int cols;
    MPI_Bcast(&cols, 1, MPI_INT, MAIN_PROC, mCommunicator);

    Matrix<float> localImg(pixels / cols, cols);
    MPI_Scatterv(NULL, NULL, NULL, MPI_FLOAT,
        localImg.ptr(), localImg.size(), MPI_FLOAT, MAIN_PROC, mCommunicator);

    mProcess->setImg(localImg);
    mProcess->initialize();
}

void SecondaryImgReconstructProcess::finalize()
{
    Matrix<float>& img = *mProcess->getImgPtr();
    int cols = img.cols();
    float* imgArray = img.ptr() + cols;
    int imgArraySize = (img.size() - 2 * cols);
    
    MPI_Gatherv(imgArray, imgArraySize, MPI_FLOAT,
        NULL, NULL, NULL, MPI_FLOAT,
        MAIN_PROC, mCommunicator);
    //printf("P%d: local image send!\n", mRank);
}
