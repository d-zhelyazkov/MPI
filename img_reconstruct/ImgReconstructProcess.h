#pragma once
#include "../tools/Tools.h"
#include "../tools/Matrix.h"
#include "../tools/Process.h"
#include <vector>

#define MAIN_PROC 0
#define THRESH 255

class ImgReconstructProcess : public Process
{
private:
    Matrix<float>* mBuffImg = NULL;
    Matrix<float>* mOriginalImg = NULL;
    Matrix<float>* mProcessedImg = NULL;

public:
    ~ImgReconstructProcess() {
        std::vector<Matrix<float>*> pointers = 
            { mOriginalImg , mProcessedImg, mBuffImg};
        deleteObjects(pointers);
    }

    void setImg(Matrix<float>& img) {
        mOriginalImg = new Matrix<float>(img);
    }

    Matrix<float>* getImgPtr() {
        return mProcessedImg;
    }

    virtual void initialize();

    virtual void processData();

    virtual void finalize() {};

    virtual Process* clone() {
        return new ImgReconstructProcess();
    }
};

