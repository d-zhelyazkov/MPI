#pragma once
#include "Process.h"
#include <utility>

template<typename T, typename std::enable_if<std::is_base_of<Process, T>::value>::type* = nullptr>
class ProcessDecorator : public Process
{
protected:
    T mProcess;

public:
    ProcessDecorator(T& process) {
        mProcess = T(process);
    }
    ~ProcessDecorator() {
        delete mProcess;
    }

    virtual void initialize() {
        mProcess->initialize();
    }

    virtual void syncData() {
        mProcess->syncData();
    }

    virtual void processData() {
        mProcess->processData();
    }

    virtual int getRank() {
        return mProcess->getRank();
    }
};

