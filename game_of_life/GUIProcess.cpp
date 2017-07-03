#include "GUIProcess.h"
#include "Commons.h"
#include <algorithm>    // std::max

void GUIProcess::initialize()
{
    printf("GUI: Process created.\n");

    int processes;
    MPI_Comm_size(MPI_COMM_WORLD, &processes);
    processes--;

    mProcesses.resize(processes);

    int globalWidth = 0;
    int globalHeight = 0;

    for (int i = 0; i < processes; i++) {

        int props[GUI_PROPS];
        MPI_Recv(props, GUI_PROPS, MPI_INT,
            i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        mProcesses[i].rank = i;
        mProcesses[i].X = props[0];
        mProcesses[i].Y = props[1];

        int width = props[2];
        int height = props[3];
        mProcesses[i].board = new Matrix<char>(height, width);

        globalWidth = std::max(globalWidth, width + mProcesses[i].X);
        globalHeight = std::max(globalHeight, height + mProcesses[i].Y);
    }

    mSDLWindow = SDL_CreateWindow("Game of Life",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        globalWidth, globalHeight,
        SDL_WINDOW_SHOWN
    );
    mSDLRenderer = SDL_CreateRenderer(mSDLWindow, -1, SDL_RENDERER_ACCELERATED);

    sendDataRequests();
}

void GUIProcess::processData()
{
    SDL_Event e;
    do {
        int dataReceived = true;
        for (WorkProcDescriptor& process : mProcesses) {
            MPI_Test(&process.recvRequest, &dataReceived, MPI_STATUS_IGNORE);
            if (!dataReceived) {
                break;
            }
        }

        //printf("GUI: All data collected.\n");
        if (dataReceived) {
            // Clear winow
            SDL_SetRenderDrawColor(mSDLRenderer, 0, 0, 0, 0);
            SDL_RenderClear(mSDLRenderer);

            SDL_SetRenderDrawColor(mSDLRenderer, 255, 255, 255, 0);
            for (WorkProcDescriptor& process : mProcesses) {
                Matrix<char>& board = *process.board;
                for (int y = 0; y < board.rows(); y++) {
                    for (int x = 0; x < board.cols(); x++) {
                        if (board(y, x)) {
                            SDL_RenderDrawPoint(mSDLRenderer, x + process.X, y + process.Y);
                        }
                    }
                }
            }
            SDL_RenderPresent(mSDLRenderer);

            sendDataRequests();
        }

        SDL_PollEvent(&e);
    } while (e.type != SDL_QUIT);
}

void GUIProcess::sendDataRequests()
{
    for (WorkProcDescriptor& process : mProcesses) {
        bool buf;
        MPI_Isend(&buf, 1, MPI_C_BOOL,
            process.rank, 0, MPI_COMM_WORLD, &process.request);
    }

    for (WorkProcDescriptor& process : mProcesses) {
        Matrix<char>& board = *process.board;
        MPI_Irecv(board.ptr(), board.size(), MPI_CHAR,
            process.rank, 0, MPI_COMM_WORLD, &process.recvRequest);
    }
}

void GUIProcess::finalize() {
    SDL_DestroyRenderer(mSDLRenderer);
    SDL_DestroyWindow(mSDLWindow);

    // Clean up
    SDL_Quit();
}