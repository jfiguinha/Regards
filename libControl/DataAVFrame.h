#pragma once
#include "PictureArray.h"


class CDataAVFrame
{
public:


	CDataAVFrame()
	{
	}

	~CDataAVFrame()
	{
		matFrame.Release();
	}
	float sample_aspect_ratio = 0.0;
	Regards::Picture::CPictureArray matFrame;
	int width;
	int height;
	float ratioVideo;

};