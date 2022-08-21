#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <vector>
#include "FaceRect.h"
using namespace std;
using namespace cv;

class CRegardsBitmap;
#ifdef __APPLE__
class MyDetectFaceImpl;
#endif

class CDetectFace
{
public:
    CDetectFace(void);
    ~CDetectFace(void);

    void LoadModel(const bool& openCLCompatible);
    void DetectFace(const Mat & bitmap, const float & confidenceThreshold, std::vector<CFace>& listOfFace, std::vector<cv::Rect>& pointOfFace);
    int FindNbFace(const Mat& bitmap, const float & confidenceThreshold, float& bestConfidence);

private:

   

#ifdef __APPLE__
    MyDetectFaceImpl* _impl;
#else
    static bool isload;
#endif
};
