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

class MyDetectFaceImpl;
class CRegardsBitmap;

class CDetectFace
{
public:
    CDetectFace ( void );
    ~CDetectFace( void );
    
    int DetectFace(CRegardsBitmap * bitmap);
    
private:
    MyDetectFaceImpl * _impl;
};

#endif