#include "SerialImgReconstructProcess.h"
#include "../tools/cio.h"
#include "Commons.h"

void SerialImgReconstructProcess::initialize()
{
    Matrix<float>* img = datread(mInputFile.c_str());
    mProcess->setImg(*img);
    
    mProcess->initialize();

    deleteObject(img);
}

void SerialImgReconstructProcess::finalize()
{
    mProcess->finalize();

    Matrix<float>* img = mProcess->getImgPtr();
    enchance(*img);
    pgmwrite(mOutputFile.c_str(), *img, THRESH);
}
