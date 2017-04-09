#pragma once
#include "MainTrafficProcess.h"
#include "SlaveTrafficProcess.h"

class TrafficProcessFactory
{
public:
    static TrafficProcess* getProccess(const MPI_Comm& communicator) {
        int rank, prevRank, nextRank;

        /* Get process rank in the communicator */
        MPI_Comm_rank(communicator, &rank);

        if (communicator == MPI_COMM_WORLD) {
            /* Get communicator size */
            int processes;
            MPI_Comm_size(communicator, &processes);

            /* Calculate neighboring ranks */
            prevRank = (rank - 1 + processes) % processes;
            nextRank = (rank + 1) % processes;
        }
        else {
            int topology;
            MPI_Topo_test(communicator, &topology);
            switch (topology)
            {
            case MPI_CART: {
                MPI_Cart_shift(communicator, 0, 1, &prevRank, &nextRank);
                break;
            }
            case MPI_GRAPH:
            case MPI_UNDEFINED:
            default:
                break;
            }
        }

        if (rank == MAIN_PROC) {
            return new MainTrafficProcess(communicator, rank, prevRank, nextRank);
        }
        else {
            return new SlaveTrafficProcess(communicator, rank, prevRank, nextRank);
        }
    }
};

