//
//  HelloInterface.h
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//

#ifdef __APPLE__
#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "FaceRect.h"
#include <vector>
using namespace std;

class MyDetectFaceImpl
{
public:
    MyDetectFaceImpl ( void );
    ~MyDetectFaceImpl( void );
    int DetectRectFace(const float &confidenceThreshold, float & best, const int &width, const int &height, uint8_t * data, vector<FaceRect> & listRect);
    int MyDetectFace(const float &confidenceThreshold, float & best, const int &width, const int &height, uint8_t * data);

};


#endif