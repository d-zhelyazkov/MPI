#include "MainImgReconstructProcess.h"
#include "cio.h"
#include "Tools.h"
#include <algorithm>

void computeProcessesWork(int processes, int work, int*& sizes, int*& offsets, int k);

MainImgReconstructProcess::MainImgReconstructProcess(const MPI_Comm & communiator, int rank, int prevProcRank, int nextProcRank)
    : ImgReconstructProcess(communiator, rank, prevProcRank, nextProcRank)
{
    Matrix<float>& wholeImg = *datread(INPUT_FILE);

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

    //spreading local images
    Matrix<float> localImg(processesPixels[MAIN_PROC] / cols, cols);
    memcpy(localImg.ptr(), wholeImg.ptr(), localImg.size() * sizeof(float));
    setImg(localImg);
    MPI_Scatterv(wholeImg.ptr(), processesPixels, processesPixelsOffsets, MPI_FLOAT,
        MPI_IN_PLACE, 0, MPI_FLOAT, MAIN_PROC, mCommunicator);
    //printf("MAIN: processes images scattered.\n");

    delete &wholeImg;
    delete[] processesPixels;
    delete[] processesPixelsOffsets;
}

void MainImgReconstructProcess::finalize()
{
    int processesCnt;
    MPI_Comm_size(mCommunicator, &processesCnt);
    int cols = mOriginalImg->cols();
    float* localImg = mProcessedImg->ptr() + cols;
    int localImgSize = (mProcessedImg->size() - 2 * cols);

    //compute global min and share it
    float* mins = new float[processesCnt];
    mins[MAIN_PROC] = arrayAbsMin(localImg, localImgSize);
    MPI_Gather(MPI_IN_PLACE, 0, MPI_FLOAT, mins, 1, MPI_FLOAT, MAIN_PROC, mCommunicator);
    float globalMin = arrayAbsMin(mins, processesCnt);
    MPI_Bcast(&globalMin, 1, MPI_FLOAT, MAIN_PROC, mCommunicator);
    delete[] mins;

    //compute global max and share it
    float* maxes = new float[processesCnt];
    maxes[MAIN_PROC] = arrayAbsMax(localImg, localImgSize);
    MPI_Gather(MPI_IN_PLACE, 0, MPI_FLOAT, maxes, 1, MPI_FLOAT, MAIN_PROC, mCommunicator);
    float globalMax = arrayAbsMax(maxes, processesCnt);
    MPI_Bcast(&globalMax, 1, MPI_FLOAT, MAIN_PROC, mCommunicator);
    delete[] maxes;

    encahnceImg(localImg, localImgSize, globalMin, globalMax, THRESH);

    //Compute processes rows from the img
    int* processesPixels;
    int* processesRixelOffsets;
    computeProcessesWork(processesCnt, mWholeImgRows, processesPixels, processesRixelOffsets, cols);

    //constructing whole image
    Matrix<float> wholeImage(mWholeImgRows, cols);
    memcpy(wholeImage.ptr(), localImg, localImgSize * sizeof(float));
    //printf("MAIN: will gather whole image.\n");
    MPI_Gatherv(MPI_IN_PLACE, 0, MPI_FLOAT,
        wholeImage.ptr(), processesPixels, processesRixelOffsets, MPI_FLOAT,
        MAIN_PROC, mCommunicator);
    delete[] processesPixels;
    delete[] processesRixelOffsets;
    //printf("MAIN: whole image gathered.\n");

   /* float* wholeImgArr = wholeImage.ptr();
    int wholeImgSize = wholeImage.size();
    float min = arrayAbsMin(wholeImgArr, wholeImgSize);
    float max = arrayAbsMax(wholeImgArr, wholeImgSize);
    encahnceImg(wholeImgArr, wholeImgSize, min, max, THRESH);*/

    //writing to file
    pgmwrite(OUTPUT_FILE, wholeImage, THRESH);

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
