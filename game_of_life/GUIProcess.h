#pragma once
#include "../tools/Process.h"
#include <vector>
#include <mpi.h>
#include "../tools/Matrix.h"
#include <SDL.h>


/*
* GUI process that collects local boards of the worker processes 
*   and displays them on a screen. 
*/
class GUIProcess :
    public Process
{
private:
    /*
    * Holder for data related to a single worker process.
    */
    struct WorkProcDescriptor {
        int rank;

        /*
        * The local board's screen position.
        */
        int X, Y;

        Matrix<char>* board = NULL;
        MPI_Request request;

        ~WorkProcDescriptor() {
            deleteObject(board);
        }
    };

    std::vector<WorkProcDescriptor> mProcesses;
    std::vector<MPI_Request> mRecvRequests;

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
    /*
    * Requests data from worker processes.
    */
    void sendDataRequests();
};

