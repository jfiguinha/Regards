#pragma once
#include "PictureArray.h"

class CDataAVFrame
{
public:


	CDataAVFrame()
	{
	}

	virtual ~CDataAVFrame()
	{
		matFrame.Release();
		if (dst != nullptr)
		{
			av_frame_unref(dst);
			dst = nullptr;
		}

	}
	float sample_aspect_ratio = 0.0;
	Regards::Picture::CPictureArray matFrame;
	int width;
	int height;
	bool isHardwareDecoding = false;
	float ratioVideo;
	AVFrame* dst = nullptr;
};