#pragma once
#include "ProcessDecorator.h"
#include "Tools.h"
#include <vector>
#include "TimeProvider.h"

using std::vector;

/*
* Keeps a track of total time spend over several calls(iterations)
*/
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

/*
* Keeps a timing track of a process's method
*/
class ProcessMethodTimeTracker : public TimeTracker {
private:
    Process* mProcess = NULL;
    void(Process::*mTrackedMethod)();
    TimeProvider* mTimeProvider;

public:
    ProcessMethodTimeTracker(
        Process* process,
        void (Process::*trackedMethod)(),
        TimeProvider* timeProvider)

        : mProcess(process) ,
        mTrackedMethod(trackedMethod) {
        
        mTimeProvider = timeProvider->clone();
    }

    ~ProcessMethodTimeTracker() {
        deleteObject(mTimeProvider);
    }

    void callFunction();

};

enum ProcessMethod{INITIALIZATION, PROCESSING, FINALIZATION, SYS_CNT};

/*
* Keeps a time trackers of all Process methods. 
*/
class ProcessTimeTracker :
    public ProcessDecorator
{
protected:
    vector<ProcessMethodTimeTracker*> mTrackers = vector<ProcessMethodTimeTracker*>(ProcessMethod::SYS_CNT);
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
    
    TimeTracker* getTracker(ProcessMethod method) {
        return new TimeTracker(*mTrackers[method]);
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

