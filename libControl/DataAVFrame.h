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
		matFrame.release();
		if (dst != nullptr)
		{
			av_free(dst);
			dst = nullptr;
		}

	}
	float sample_aspect_ratio = 0.0;
	cv::Mat matFrame;
	int width;
	int height;
	bool isHardwareDecoding = false;
	float ratioVideo;
	AVFrame* dst = nullptr;
};