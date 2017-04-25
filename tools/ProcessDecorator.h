#pragma once
#include "Process.h"
#include "Tools.h"

class ProcessDecorator : public Process
{
protected:
    Process* mBaseProcess = NULL;

public:
    ProcessDecorator(Process& process) {
        mBaseProcess = process.clone();
    }

    ProcessDecorator(ProcessDecorator& processDecorator) 
        : ProcessDecorator(*(processDecorator.mBaseProcess)){}

    ~ProcessDecorator() {
       deleteObject(mBaseProcess);
    }

    virtual void initialize() {
        mBaseProcess->initialize();
    }
    
    virtual void processData() {
        mBaseProcess->processData();
    }

    virtual void finalize() {
        mBaseProcess->finalize();
    }
};

