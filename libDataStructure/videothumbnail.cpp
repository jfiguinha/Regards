// IBitmap.h: interface for the CRegardsBitmap class.
//
//////////////////////////////////////////////////////////////////////
#include "videothumbnail.h"

CVideoThumbnail::CVideoThumbnail()
{
	copy = false;
	for (auto i = 0; i < NBELEMENT; i++)
	{
		bitmap[i] = nullptr;

	}
}

CVideoThumbnail::~CVideoThumbnail()
{
	if (!copy)
	{
		for (auto i = 0; i < NBELEMENT; i++)
		{
			if (bitmap[i] != nullptr)
			{
				delete bitmap[i];
				bitmap[i] = nullptr;
			}
		}
	}

}

void CVideoThumbnail::SetCopy(const bool& copy)
{
	this->copy = copy;
}
