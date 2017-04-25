#pragma once

class Process
{
public:

    virtual void initialize() = 0;

    virtual void processData() = 0;

    virtual void finalize() = 0;

    virtual Process* clone() = 0;
};

