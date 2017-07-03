#pragma once

class TimeProvider {

public:

    /**
    * Gets the time in seconds.
    */
    virtual double getTime() = 0;

    virtual TimeProvider* clone() = 0;
};