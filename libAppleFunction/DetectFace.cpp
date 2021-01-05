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
#endif