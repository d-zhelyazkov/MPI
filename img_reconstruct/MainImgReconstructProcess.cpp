#include "MainImgReconstructProcess.h"
#include "../tools/cio.h"
#include "../tools/Tools.h"
#include "Commons.h"

void computeProcessesWork(int processes, int work, int*& sizes, int*& offsets, int k);

void MainImgReconstructProcess::initialize()
{
    Matrix<float>& wholeImg = *datread(mInputFile.c_str());

    /* Get communicator size */
    int processes;
    MPI_Comm_size(mCommunicator, &processes);
    //Compute processes rows from the img
    int* processesPixels;
    int* processesPixelsOffsets;
    mWholeImgRows = wholeImg.rows();
    int cols = wholeImg.cols();
    computeProcessesWork(processes, mWholeImgRows, processesPixels, processesPixelsOffsets, cols);

    //spreading local images' sizes
    MPI_Scatter(processesPixels, 1, MPI_INT,
        MPI_IN_PLACE, 0, MPI_INT, MAIN_PROC, mCommunicator);
    MPI_Bcast(&cols, 1, MPI_INT, MAIN_PROC, mCommunicator);
    //printf("MAIN: processes image sizes spread.\n");

    //spreading local part images
    MPI_Scatterv(wholeImg.ptr(), processesPixels, processesPixelsOffsets, MPI_FLOAT,
        MPI_IN_PLACE, 0, MPI_FLOAT, MAIN_PROC, mCommunicator);
    //printf("MAIN: processes images scattered.\n");

    Matrix<float> localImg(processesPixels[MAIN_PROC] / cols, cols);
    memcpy(localImg.ptr(), wholeImg.ptr(), localImg.size() * sizeof(float));
    mProcess->setImg(localImg);
    mProcess->initialize();

    deleteArray(processesPixels);
    deleteArray(processesPixelsOffsets);
    delete &wholeImg;
}

void MainImgReconstructProcess::finalize()
{
    Matrix<float>& img = *mProcess->getImgPtr();

    int processesCnt;
    MPI_Comm_size(mCommunicator, &processesCnt);
    int cols = img.cols();
    float* imgArr = img.ptr() + cols;
    int imgArrSize = (img.size() - 2 * cols);
    
    //Compute processes rows from the img
    int* processesPixels;
    int* processesPixelOffsets;
    computeProcessesWork(processesCnt, mWholeImgRows, processesPixels, processesPixelOffsets, cols);

    //constructing whole image
    Matrix<float> wholeImage(mWholeImgRows, cols);
    //printf("MAIN: will gather whole image.\n");
    MPI_Gatherv(MPI_IN_PLACE, 0, MPI_FLOAT,
        wholeImage.ptr(), processesPixels, processesPixelOffsets, MPI_FLOAT,
        MAIN_PROC, mCommunicator);
    //printf("MAIN: whole image gathered.\n");

    float* wholeImgArr = wholeImage.ptr();
    copyArray(wholeImgArr, imgArr, imgArrSize);
    
    enchance(wholeImage);

    //writing to file
    pgmwrite(mOutputFile.c_str(), wholeImage, THRESH);

    deleteArray(processesPixels);
    deleteArray(processesPixelOffsets);
}

void computeProcessesWork(int processes, int work, int*& sizes, int*& offsets, int k) {
    sizes = new int[processes];
    offsets = new int[processes];
    int currOffset = 0;
    int reminders = work % processes;
    int N = work / processes;
    for (int i = 0; i < processes; i++) {
        int size = (i < reminders) ? N + 1 : N;
        size *= k;
        sizes[i] = size;
        offsets[i] = currOffset;
        //printf("%d:\twork: %d\toffset: %d\n", i, size, currOffset);
        currOffset += size;
    }
}
