#include "SerialGLProcess.h"
#include "Commons.h"

void SerialGLProcess::initialize()
{
    Matrix<char>* inputData = readFile(mInputFile);
    Matrix<char>* board = convertToBool(*inputData);
    mProcess->setBoard(*board);

    ProcessDecorator::initialize();

    deleteObject(inputData);
    deleteObject(board);
}

void SerialGLProcess::finalize()
{
    ProcessDecorator::finalize();

    Matrix<char>* board = mProcess->getBoard();
    Matrix<char>* outputData = convertToChar(*board);
    writeFile(mOutputFile, *outputData);

    deleteObject(outputData);
    deleteObject(board);
}
