#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <vector>
using namespace std;

class CRegardsBitmap;
#ifdef __APPLE__
class MyDetectFaceImpl;
#endif

class CFace
{
public:
    float confidence;
    cv::Mat croppedImage;
    cv::Rect myROI;
};

class CDetectFace
{
public:
    CDetectFace(void);
    ~CDetectFace(void);

    static bool LockOpenCLDnn();
    static bool UnlockOpenCLDnn();

    void LoadModel(const string& config_file, const string& weight_file);
    void DetectFace(CRegardsBitmap* bitmap, const float & confidenceThreshold, std::vector<CFace>& listOfFace, std::vector<cv::Rect>& pointOfFace);
    int FindNbFace(CRegardsBitmap* bitmap, const float & confidenceThreshold, float& bestConfidence);

private:

    

#ifdef __APPLE__
    MyDetectFaceImpl* _impl;
#else
    static bool isload;
#endif
};
