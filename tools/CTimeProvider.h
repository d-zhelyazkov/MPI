#pragma once

#include "TimeProvider.h"
#include <time.h>

class CTimeProvider : public TimeProvider {

public:

    double getTime() {
        return (double)clock() / CLOCKS_PER_SEC;
    }

    CTimeProvider* clone() {
        return new CTimeProvider();
    }
};