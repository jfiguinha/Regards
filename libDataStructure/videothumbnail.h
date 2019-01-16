// IBitmap.h: interface for the CRegardsBitmap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "RegardsBitmap.h"
#define NBELEMENT 21


class CVideoThumbnail
{
public:

	CVideoThumbnail();
	~CVideoThumbnail();

	void SetCopy(const bool& copy);

	CRegardsBitmap * bitmap[NBELEMENT];
	int rotation;
	int timePosition[NBELEMENT];
	bool copy;
};


