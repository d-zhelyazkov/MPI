#pragma once
#include "TimeTracker.h"

class MainProcess : public ProcessTimeTracker {

    int mIterations = 0;

public:
    MainProcess(Process& process) : ProcessTimeTracker(process) {}

    virtual void processData() {
        ProcessTimeTracker::processData();
        mIterations++;
        if (!(mIterations % 1000))
            printf("%d iterations done\n", mIterations);
    }

    virtual void finalize() {
        ProcessTimeTracker::finalize();

        printf("Init time: %fs\n", mTrackers[INITIALIZATION]->getTotalTime());
        TimeTracker* processing = mTrackers[PROCESSING];
        printf("Processing time: %fs\nAvg iteration time: %fs\n",
            processing->getTotalTime(), processing->getAvgTime());
        printf("Finalize time: %fs\n", mTrackers[FINALIZATION]->getTotalTime());
    }
};