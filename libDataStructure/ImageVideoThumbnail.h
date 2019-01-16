//
//  CxImageVideoThumbnail.h
//  Regards.libDataStructure
//
//  Created by figuinha jacques on 11/08/2015.
//  Copyright (c) 2015 figuinha jacques. All rights reserved.
//

#pragma once
#include "ImageLoadingFormat.h"

#if (defined(__APPLE__) || defined(WIN32)) && !defined(FFMPEG)

class CRegardsBitmap;

class CImageVideoThumbnail
{
public:
    
	CImageVideoThumbnail()
	{
		image = nullptr;
		rotation = 0;
		percent = 0;
		timePosition = 0;
	};
	~CImageVideoThumbnail(){};
    
    CRegardsBitmap * image;
    int rotation;
    int percent;
    int timePosition;
};

#elif defined(FFMPEG)

class CImageLoadingFormat;

class CImageVideoThumbnail
{
public:
    
	CImageVideoThumbnail()
	{
		image = nullptr;
		rotation = 0;
		percent = 0;
		delay = 50;
		timePosition = 0;
	};
	~CImageVideoThumbnail()
	{
		if(image != nullptr)
			delete image;
	};
    
    CImageLoadingFormat * image;
    int rotation;
	uint32_t delay;
    int percent;
    int timePosition;
};
#endif