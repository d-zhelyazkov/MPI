#include "TimeTracker.h"

void ProcessMethodTimeTracker::callFunction()
{

    double begin = mTimeProvider->getTime();
    (mProcess->*mTrackedMethod)();
    double time = mTimeProvider->getTime() - begin;

    mTotalTime += time;
    mCalls++;
}

void ProcessTimeTracker::init()
{
    mTrackers[INITIALIZATION] = 
        new ProcessMethodTimeTracker(mBaseProcess, &Process::initialize, mTimeProvider);
    mTrackers[PROCESSING] = 
        new ProcessMethodTimeTracker(mBaseProcess, &Process::processData, mTimeProvider);
    mTrackers[FINALIZATION] = 
        new ProcessMethodTimeTracker(mBaseProcess, &Process::finalize, mTimeProvider);
}
