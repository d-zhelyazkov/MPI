#pragma once
#include "TimeTracker.h"

#define ITER_DEF_NOTIFICATION 1000

class MainProcess : public ProcessTimeTracker {

private:
    unsigned mIterations = 0;
    unsigned mIterationNotification = ITER_DEF_NOTIFICATION;
    double mProcessStartTime;

public:
    MainProcess(Process& process, TimeProvider* timeProvider) 
        : ProcessTimeTracker(process, timeProvider) {}

    MainProcess(
        Process& process,
        unsigned iterationNotification,
        TimeProvider* timeProvider)

        : ProcessTimeTracker(process, timeProvider),
        mIterationNotification(iterationNotification) {}

    virtual void initialize() {
        mProcessStartTime = mTimeProvider->getTime();

        ProcessTimeTracker::initialize();
    }

    virtual void processData() {
        ProcessTimeTracker::processData();
        mIterations++;
        if (!(mIterations % mIterationNotification))
            printf("%d iterations done\n", mIterations);
    }

    virtual void finalize() {
        ProcessTimeTracker::finalize();
        double processTotalTime = mTimeProvider->getTime() - mProcessStartTime;

        printf("Init time: %fs\n", mTrackers[INITIALIZATION]->getTotalTime());
        TimeTracker* processing = mTrackers[PROCESSING];
        printf("Processing time: %fs\nAvg iteration time: %fs\n",
            processing->getTotalTime(), processing->getAvgTime());
        printf("Finalize time: %fs\n", mTrackers[FINALIZATION]->getTotalTime());
        printf("Total process time: %fs\n", processTotalTime);
    }
};