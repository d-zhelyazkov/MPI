#pragma once
#include "../tools/Process.h"
#include <vector>
#include <mpi.h>
#include "../tools/Matrix.h"


class GUIProcess :
    public Process
{
private:
    struct WorkProcDescriptor {
        int rank;
        int X;
        int Y;
        Matrix<char>* board = NULL;
        MPI_Request request;

        ~WorkProcDescriptor() {
            deleteObject(board);
        }
    };

    std::vector<WorkProcDescriptor> mProcesses;

public:

    void initialize();

    void processData();

    void finalize() {}

    GUIProcess* clone() {
        return new GUIProcess();
    }

private:
    void sendDataRequests();
};

