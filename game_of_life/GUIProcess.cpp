#include "GUIProcess.h"
#include "Commons.h"


void GUIProcess::initialize()
{
    printf("GUI: Process created.\n");

    int processes;
    MPI_Comm_size(MPI_COMM_WORLD, &processes);
    processes--;

    mProcesses.resize(processes);

    for (int i = 0; i < processes; i++) {

        int props[GUI_PROPS];
        MPI_Recv(props, GUI_PROPS, MPI_INT,
            i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        mProcesses[i].rank = i;
        mProcesses[i].X = props[0];
        mProcesses[i].Y = props[1];
        mProcesses[i].board = new Matrix<char>(props[3], props[2]);
    }

    sendDataRequests();
}

void GUIProcess::processData()
{
    while (true) {
        for (WorkProcDescriptor& process : mProcesses) {
            Matrix<char>& board = *process.board;
            MPI_Recv(board.ptr(), board.size(), MPI_CHAR,
                process.rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        printf("GUI: All data collected.\n");

        sendDataRequests();
    }
}

void GUIProcess::sendDataRequests()
{
    for (WorkProcDescriptor& process : mProcesses) {
        bool buf;
        MPI_Isend(&buf, 1, MPI_C_BOOL,
            process.rank, 0, MPI_COMM_WORLD, &process.request);
    }
}
