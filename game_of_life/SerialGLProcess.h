#pragma once
#include "../tools/ProcessDecorator.h"
#include <string>
#include "GLProcess.h"

using std::string;

/*
* Simple GoL Process. Implements the following functionalities:
*   - reads the pattern from the input file
*   - writes the pattern to the output file
*/
class SerialGLProcess :
    public ProcessDecorator
{
private:
    string mInputFile;
    string mOutputFile;
    GLProcess* mProcess;

public:
    SerialGLProcess(GLProcess& process, string& inputFile, string& outputFile)
        : ProcessDecorator(process),
        mInputFile(inputFile),
        mOutputFile(outputFile) {

        mProcess = (GLProcess*)mBaseProcess;
    }

    virtual void initialize();

    virtual void finalize();

    virtual SerialGLProcess* clone() {
        return new SerialGLProcess(*mProcess, mInputFile, mOutputFile);
    }
};

