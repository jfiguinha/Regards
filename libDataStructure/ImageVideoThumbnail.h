//
//  CxImageVideoThumbnail.h
//  Regards.libDataStructure
//
//  Created by figuinha jacques on 11/08/2015.
//  Copyright (c) 2015 figuinha jacques. All rights reserved.
//

#pragma once

#if defined(__APPLE__) || defined(WIN32)

class CRegardsBitmap;

class CImageVideoThumbnail
{
public:
    
	CImageVideoThumbnail(){};
	~CImageVideoThumbnail(){};
    
    CRegardsBitmap * image = nullptr;
    int rotation = 0;
    int percent = 0;
    int timePosition = 0;
};

#else

class CxImage;

class CImageVideoThumbnail
{
public:
    
	CImageVideoThumbnail(){};
	~CImageVideoThumbnail(){};
    
    CxImage * image = nullptr;
    int rotation = 0;
    int percent = 0;
    int timePosition = 0;
};
#endif