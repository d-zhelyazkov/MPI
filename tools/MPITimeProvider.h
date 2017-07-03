#pragma once

#include "TimeProvider.h"
#include <mpi.h>

class MPITimeProvider : public TimeProvider {
public:

    double getTime() {
        return MPI_Wtime();
    }

    MPITimeProvider* clone() {
        return new MPITimeProvider();
    }
};