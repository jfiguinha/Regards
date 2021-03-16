//
//  Hello.cpp
//  Regards
//
//  Created by figuinha jacques on 10/08/2015.
//
//

#ifdef __APPLE__
#include "DetectFace.h"
#include "MyDetectFaceImpl.h"
#include <RegardsBitmap.h>

CDetectFace::CDetectFace( void )
{
    _impl = new MyDetectFaceImpl();
}


CDetectFace::~CDetectFace( void )
{
    if ( _impl ) { delete _impl; _impl = NULL; }
}


int CDetectFace::DetectFace(CRegardsBitmap * bitmap)
{
	int faceDetect = 0;
    if ( _impl )
        faceDetect = _impl->MyDetectFace(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap());
	return faceDetect;
}

int CDetectFace::DetectRectFace(CRegardsBitmap * bitmap, vector<FaceRect> & listFaceRect)
{
    int faceDetect = 0;
    if ( _impl )
        faceDetect = _impl->DetectRectFace(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap(), listFaceRect);
	return faceDetect;
}

int CDetectFace::GetExifOrientation(CRegardsBitmap * bitmap)
{
    int angle = 0;
    if ( _impl )
    {
        for(int i = 0;i < 4;i++)
        {
            int numface =  _impl->MyDetectFace(bitmap->GetBitmapWidth(), bitmap->GetBitmapHeight(), bitmap->GetPtBitmap());
            if(numface > 0)
                break;
            angle += 90;
            bitmap->Rotation90();
        }      
    }

    return RotateToExifOrientation(angle);
}

int CDetectFace::RotateToExifOrientation(const int &angle)
{
	if (angle > 44 && angle < 135) //Rotate 90
		return 8;
	else if (angle > 134 && angle < 225) //Rotate 180
		return 3;
	else if (angle > 224 && angle < 315)
		return 6;
	return 0;
}

#endif