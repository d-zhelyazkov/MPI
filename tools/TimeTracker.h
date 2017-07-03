#pragma once
#include "ProcessDecorator.h"
#include "Tools.h"
#include <vector>
#include "TimeProvider.h"

using std::vector;

class TimeTracker {
protected:
    double mTotalTime = 0;
    int mCalls = 0;

public:

    int getCalls() {
        return mCalls;
    }

    double getTotalTime() {
        return mTotalTime;
    }

    double getAvgTime() {
        return mTotalTime / mCalls;
    }
};

class ProcessMethodTimeTracker : public TimeTracker{
private:
    Process* mProcess = NULL;
    void(Process::*mTrackedMethod)();
    TimeProvider* mTimeProvider;

public:
    ProcessMethodTimeTracker(
        Process* process,
        void (Process::*trackedMethod)(),
        TimeProvider* timeProvider) :

        mProcess(process) ,
        mTrackedMethod(trackedMethod) {
        
        mTimeProvider = timeProvider->clone();
    }

    ~ProcessMethodTimeTracker() {
        deleteObject(mTimeProvider);
    }

    void callFunction();

};


enum ProcessTrackerType{INITIALIZATION, PROCESSING, FINALIZATION, SYS_CNT};

class ProcessTimeTracker :
    public ProcessDecorator
{
protected:
    vector<ProcessMethodTimeTracker*> mTrackers = vector<ProcessMethodTimeTracker*>(ProcessTrackerType::SYS_CNT);
    TimeProvider* mTimeProvider;

public:
    ProcessTimeTracker(Process& process, TimeProvider* timeProvider)
        : ProcessDecorator(process) {

        mTimeProvider = timeProvider->clone();
        init();
    };

    ProcessTimeTracker(ProcessTimeTracker& processTracker) :
        ProcessDecorator(processTracker),
        mTimeProvider(processTracker.mTimeProvider) {

        init();
    }
    
    TimeTracker* getTracker(ProcessTrackerType trackerType) {
        return new TimeTracker(*mTrackers[trackerType]);
    }

    ~ProcessTimeTracker() {
        deleteObjects(mTrackers);
    }

    virtual void initialize() {
        mTrackers[INITIALIZATION]->callFunction();
    }
    
    virtual void processData() {
        mTrackers[PROCESSING]->callFunction();
    }

    virtual void finalize() {
        mTrackers[FINALIZATION]->callFunction();
    };

    virtual ProcessTimeTracker* clone() {
        return new ProcessTimeTracker(*this);
    }

private:
    void init();
};

