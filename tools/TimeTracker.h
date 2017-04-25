#pragma once
#include "ProcessDecorator.h"
#include <time.h>
#include "Tools.h"
#include <vector>

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

public:
    ProcessMethodTimeTracker(Process* process, void (Process::*trackedMethod)()) :
        mProcess(process) ,
        mTrackedMethod(trackedMethod) {}

    void callFunction();

};


enum ProcessTrackerType{INITIALIZATION, PROCESSING, FINALIZATION, SYS_CNT};

class ProcessTimeTracker :
    public ProcessDecorator
{
protected:
    vector<ProcessMethodTimeTracker*> mTrackers = vector<ProcessMethodTimeTracker*>(ProcessTrackerType::SYS_CNT);

public:
    ProcessTimeTracker(Process& process) : ProcessDecorator(process) {
        init();
    };

    ProcessTimeTracker(ProcessTimeTracker& processTracker) :
        ProcessDecorator(processTracker) {

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

