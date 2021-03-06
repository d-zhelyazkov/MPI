#include <stdlib.h>
#include "SerialGLProcess.h"
#include "Commons.h"
#include "../tools/MainProcess.h"
#include "../tools/CTimeProvider.h"

int main(int argc, char **argv)
{
    checkARGV(argc, argv);

    int iterations = atoi(argv[2]);
    string inputFile = string(argv[1]);
    string& outputFile = *getOutputFileName(inputFile, iterations, "serial");

    GLProcess glProcess;
    SerialGLProcess serialProcess(glProcess, inputFile, outputFile);
    CTimeProvider timeProvider;
    MainProcess mainProcess(serialProcess, ITER_NOTIFICATION, &timeProvider);

    mainProcess.initialize();
    for (int k = 1; k <= iterations; k++) {
        mainProcess.processData();
    }
    mainProcess.finalize();

    deleteObject(outputFile);
    return 0;
}