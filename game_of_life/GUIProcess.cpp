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
    mRecvRequests.resize(processes);

    int globalWidth = 0;
    int globalHeight = 0;

    for (int i = 0; i < processes; i++) {

        //receives a working process initial parameters
        int props[GUI_PROPS];
        MPI_Recv(props, GUI_PROPS, MPI_INT,
            i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        mProcesses[i].rank = i;
        mProcesses[i].X = props[0];
        mProcesses[i].Y = props[1];

        int width = props[2];
        int height = props[3];
        mProcesses[i].board = new Matrix<char>(height, width);

        //determines global pattern size
        globalWidth = std::max(globalWidth, width + mProcesses[i].X);
        globalHeight = std::max(globalHeight, height + mProcesses[i].Y);
    }

    mSDLWindow = SDL_CreateWindow(
        "Game of Life",
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
        //checks if all work processes have send their local data
        int dataReceived = true;
        MPI_Testall(mRecvRequests.size(), &mRecvRequests[0],
            &dataReceived, MPI_STATUSES_IGNORE);

        if (dataReceived) {
            // Clear winow - black
            SDL_SetRenderDrawColor(mSDLRenderer, 0, 0, 0, 0);
            SDL_RenderClear(mSDLRenderer);

            //white draw color
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
    //until quit button has been clicked
    } while (e.type != SDL_QUIT);
}

void GUIProcess::sendDataRequests()
{
    for (WorkProcDescriptor& process : mProcesses) {
        //asynchronously sends data request to every work process
        bool buf;
        MPI_Isend(&buf, 1, MPI_C_BOOL,
            process.rank, 0, MPI_COMM_WORLD, &process.request);
    }

    for (WorkProcDescriptor& process : mProcesses) {
        //asynchronously receives data from every work process
        Matrix<char>& board = *process.board;
        MPI_Irecv(board.ptr(), board.size(), MPI_CHAR,
            process.rank, 0, MPI_COMM_WORLD, &mRecvRequests[process.rank]);
    }
}

void GUIProcess::finalize() {

    // Clean up
    SDL_DestroyRenderer(mSDLRenderer);
    SDL_DestroyWindow(mSDLWindow);
    SDL_Quit();
}