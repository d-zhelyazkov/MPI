#include "MainImgReconstructProcess.h"
#include "../tools/cio.h"
#include "../tools/Tools.h"

void computeProcessesWork(int processes, int work, int*& sizes, int*& offsets, int k);

void MainImgReconstructProcess::initialize()
{
    Matrix<float>& wholeImg = *datread(mInputFile);
    MPI_Comm& comm = *mProcess->getCommunicator();

    /* Get communicator size */
    int processes;
    MPI_Comm_size(comm, &processes);
    //Compute processes rows from the img
    int* processesPixels;
    int* processesPixelsOffsets;
    mWholeImgRows = wholeImg.rows();
    int cols = wholeImg.cols();
    computeProcessesWork(processes, mWholeImgRows, processesPixels, processesPixelsOffsets, cols);

    //spreading local images' sizes
    MPI_Scatter(processesPixels, 1, MPI_INT,
        MPI_IN_PLACE, 0, MPI_INT, MAIN_PROC, comm);
    MPI_Bcast(&cols, 1, MPI_INT, MAIN_PROC, comm);
    //printf("MAIN: processes image sizes spread.\n");

    //spreading local part images
    MPI_Scatterv(wholeImg.ptr(), processesPixels, processesPixelsOffsets, MPI_FLOAT,
        MPI_IN_PLACE, 0, MPI_FLOAT, MAIN_PROC, comm);
    //printf("MAIN: processes images scattered.\n");

    Matrix<float> localImg(processesPixels[MAIN_PROC] / cols, cols);
    memcpy(localImg.ptr(), wholeImg.ptr(), localImg.size() * sizeof(float));
    mProcess->setImg(localImg);
    mProcess->initialize();

    delete &wholeImg;
    delete[] processesPixels;
    delete[] processesPixelsOffsets;
    delete &comm;
}

void MainImgReconstructProcess::finalize()
{
    MPI_Comm& comm = *mProcess->getCommunicator();
    Matrix<float>& img = *mProcess->getImg();

    int processesCnt;
    MPI_Comm_size(comm, &processesCnt);
    int cols = img.cols();
    float* imgArr = img.ptr() + cols;
    int imgArrSize = (img.size() - 2 * cols);

    //compute global min and share it
    float* mins = new float[processesCnt];
    mins[MAIN_PROC] = arrayAbsMin(imgArr, imgArrSize);
    MPI_Gather(MPI_IN_PLACE, 0, MPI_FLOAT, mins, 1, MPI_FLOAT, MAIN_PROC, comm);
    float globalMin = arrayAbsMin(mins, processesCnt);
    MPI_Bcast(&globalMin, 1, MPI_FLOAT, MAIN_PROC, comm);
    delete[] mins;

    //compute global max and share it
    float* maxes = new float[processesCnt];
    maxes[MAIN_PROC] = arrayAbsMax(imgArr, imgArrSize);
    MPI_Gather(MPI_IN_PLACE, 0, MPI_FLOAT, maxes, 1, MPI_FLOAT, MAIN_PROC, comm);
    float globalMax = arrayAbsMax(maxes, processesCnt);
    MPI_Bcast(&globalMax, 1, MPI_FLOAT, MAIN_PROC, comm);
    delete[] maxes;

    encahnceImg(imgArr, imgArrSize, globalMin, globalMax, THRESH);

    //Compute processes rows from the img
    int* processesPixels;
    int* processesRixelOffsets;
    computeProcessesWork(processesCnt, mWholeImgRows, processesPixels, processesRixelOffsets, cols);

    //constructing whole image
    Matrix<float> wholeImage(mWholeImgRows, cols);
    //printf("MAIN: will gather whole image.\n");
    MPI_Gatherv(MPI_IN_PLACE, 0, MPI_FLOAT,
        wholeImage.ptr(), processesPixels, processesRixelOffsets, MPI_FLOAT,
        MAIN_PROC, comm);
    //printf("MAIN: whole image gathered.\n");
    delete[] processesPixels;
    delete[] processesRixelOffsets;

    float* wholeImgArr = wholeImage.ptr();
    copyArray(wholeImgArr, imgArr, imgArrSize);
    
    //writing to file
    pgmwrite(mOutputFile, wholeImage, THRESH);

    delete &comm;
    delete &img;
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
