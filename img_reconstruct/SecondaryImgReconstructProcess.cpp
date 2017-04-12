#include "SecondaryImgReconstructProcess.h"
#include "../tools/Tools.h"


void SecondaryImgReconstructProcess:: initialize()
{
    int pixels;

    MPI_Comm& comm = *mProcess->getCommunicator();

    MPI_Scatter(NULL, 0, MPI_INT,
        &pixels, 1, MPI_INT, MAIN_PROC, comm);
    int cols;
    MPI_Bcast(&cols, 1, MPI_INT, MAIN_PROC, comm);

    Matrix<float> localImg(pixels / cols, cols);
    MPI_Scatterv(NULL, NULL, NULL, MPI_FLOAT,
        localImg.ptr(), localImg.size(), MPI_FLOAT, MAIN_PROC, comm);

    mProcess->setImg(localImg);
    mProcess->initialize();

    delete &comm;
}

void SecondaryImgReconstructProcess::finalize()
{
    MPI_Comm& comm = *mProcess->getCommunicator();
    Matrix<float>& img = *mProcess->getImg();
    int cols = img.cols();
    float* imgArray = img.ptr() + cols;
    int imgArraySize = (img.size() - 2 * cols);

    //compute and send local min, wait for global min
    float localMin = arrayAbsMin(imgArray, imgArraySize);
    MPI_Gather(&localMin, 1, MPI_FLOAT, NULL, 0, MPI_FLOAT, MAIN_PROC, comm);
    float globalMin;
    MPI_Bcast(&globalMin, 1, MPI_FLOAT, MAIN_PROC, comm);

    //compute and send local max, wait for global max
    float localMax = arrayAbsMax(imgArray, imgArraySize);
    MPI_Gather(&localMax, 1, MPI_FLOAT, NULL, 0, MPI_FLOAT, MAIN_PROC, comm);
    float globalMax;
    MPI_Bcast(&globalMax, 1, MPI_FLOAT, MAIN_PROC, comm);

    encahnceImg(imgArray, imgArraySize, globalMin, globalMax, THRESH);

    MPI_Gatherv(imgArray, imgArraySize, MPI_FLOAT,
        NULL, NULL, NULL, MPI_FLOAT,
        MAIN_PROC, comm);
    //printf("P%d: local image send!\n", mRank);

    delete &comm;
    delete &img;
}
