#pragma once

class CDataAVFrame
{
public:


	CDataAVFrame()
	{
	}

	~CDataAVFrame()
	{
		if(matFrame != nullptr)
		{
			delete matFrame;
			matFrame = nullptr;
		}
	}
	float sample_aspect_ratio = 0.0;
	cv::Mat * matFrame = nullptr;
	int width;
	int height;
	float ratioVideo;

};