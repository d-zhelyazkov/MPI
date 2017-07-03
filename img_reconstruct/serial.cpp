#include <stdio.h>
#include <math.h>
#include "Commons.h"
#include "../tools/cio.h"
#include "../tools/Tools.h"
#include "../tools/MainProcess.h"
#include "SerialImgReconstructProcess.h"
#include "../tools/CTimeProvider.h"

int main (int argc, char **argv)
{

    checkARGV(argc, argv);

    int iterations = atoi(argv[2]);
    string inputFile = string(argv[1]);
    string* outputFile = getOutputFileName(argv[1], iterations, "serial");

    ImgReconstructProcess imgReconstructProcess;
    SerialImgReconstructProcess serialImgReconstructProcess(imgReconstructProcess, inputFile, *outputFile);
    CTimeProvider timeProvider;
    MainProcess mainProcess(serialImgReconstructProcess, &timeProvider);
    mainProcess.initialize();
    for (int k = 1; k <= iterations; k++) {
        mainProcess.processData();
    }
    mainProcess.finalize();

    deleteObject(outputFile);
    return 0;
}
