//
//  Hello.h
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

class MyDetectFaceImpl;
class CRegardsBitmap;

class CDetectFace
{
public:
    CDetectFace ( void );
    ~CDetectFace( void );

    int DetectFace(CRegardsBitmap * bitmap);
    int DetectRectFace(CRegardsBitmap * bitmap, vector<FaceRect> & listFaceRect);
    int GetExifOrientation(CRegardsBitmap * bitmap);
private:
    MyDetectFaceImpl * _impl;
    int RotateToExifOrientation(const int &angle);
};

#endif
