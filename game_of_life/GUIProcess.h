#pragma once
#include "../tools/Process.h"
#include <vector>
#include <mpi.h>
#include "../tools/Matrix.h"
#include <SDL.h>


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
        MPI_Request recvRequest;

        ~WorkProcDescriptor() {
            deleteObject(board);
        }
    };

    std::vector<WorkProcDescriptor> mProcesses;

    SDL_Window* mSDLWindow = NULL;
    SDL_Renderer* mSDLRenderer = NULL;

public:

    void initialize();

    void processData();

    void finalize();

    GUIProcess* clone() {
        return new GUIProcess();
    }

private:
    void sendDataRequests();
};

