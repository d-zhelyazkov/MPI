#pragma once
#include "ProcessDecorator.h"

template<typename T, typename std::enable_if<std::is_base_of<Process, T>::value>::type* = nullptr>
class SecondaryProcess : public ProcessDecorator<T>
{
public:
    SecondaryProcess(T& process) : MPIProcessDecorator(process) {}

    virtual void initialize() {
        mProcess->initialize();
        recieveDataFromMainProcess();
    }

    virtual void processData() {
        mProcess->processData();
        sendDataToMainProcess();
    }

    virtual void recieveDataFromMainProcess() = 0;

    virtual void sendDataToMainProcess() = 0;
};

