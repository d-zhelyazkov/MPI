#include "TimeTracker.h"

void ProcessMethodTimeTracker::callFunction()
{

    clock_t begin = clock();
    (mProcess->*mTrackedMethod)();
    clock_t time = clock() - begin;

    mTotalTime += (double)time / CLOCKS_PER_SEC;
    mCalls++;
}

void ProcessTimeTracker::init()
{
    mTrackers[INITIALIZATION] = new ProcessMethodTimeTracker(mBaseProcess, &Process::initialize);
    mTrackers[PROCESSING] = new ProcessMethodTimeTracker(mBaseProcess, &Process::processData);
    mTrackers[FINALIZATION] = new ProcessMethodTimeTracker(mBaseProcess, &Process::finalize);
}
