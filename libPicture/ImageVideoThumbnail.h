//
// 
//  Regards.libDataStructure
//
//  Created by figuinha jacques on 11/08/2015.
//  Copyright (c) 2015 figuinha jacques. All rights reserved.
//

#pragma once
#include "ImageLoadingFormat.h"


class CImageLoadingFormat;

class CImageVideoThumbnail
{
public:
	CImageVideoThumbnail()
	{
		rotation = 0;
		percent = 0;
		delay = 50;
		timePosition = 0;
	};

	~CImageVideoThumbnail()
	{
	};

	wxImage image;
	int rotation;
	uint32_t delay;
	int percent;
	int timePosition;
	wxString filename;
};
