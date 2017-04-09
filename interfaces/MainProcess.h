#pragma once
#include "ProcessDecorator.h"

template<typename T, typename std::enable_if<std::is_base_of<Process, T>::value>::type* = nullptr>
class MainProcess : public ProcessDecorator<T>
{
public:
    MainProcess(T& process) : MPIProcessDecorator(process) {}


    virtual void initialize() {
        mProcess->initialize();
        sendDataToSecProcesses();
    }

    virtual void processData() {
        mProcess->processData();
        recieveDataFromSecProcesses();
    }

    virtual void sendDataToSecProcesses() = 0;

    virtual void recieveDataFromSecProcesses() = 0;

};

