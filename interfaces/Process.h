#pragma once

class Process
{
public:
    virtual void initialize() = 0;

    virtual void syncData() = 0;

    virtual void processData() = 0;

    virtual int getRank() = 0;
};

