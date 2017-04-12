#pragma once
#include "Process.h"
#include <utility>
#include "Tools.h"

template<typename T, typename std::enable_if<std::is_base_of<Process, T>::value>::type* = nullptr>
class ProcessDecorator : public Process
{
protected:
    T* mProcess = NULL;

public:
    ProcessDecorator(T* process) {
        mProcess = new T(*process);
    }

    ~ProcessDecorator() {
       deleteObject(mProcess);
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

    virtual void finalize() {
        mProcess->finalize();
    }
};

