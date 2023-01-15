#pragma once
#include "header.h"

class CFace
{
public:
	float confidence;
	cv::Mat croppedImage;
	cv::Rect myROI;
	int angle = 0;
};

struct FaceBox
{
	int x, y, w, h;
	float angle, scale, score;

	cv::Rect getBox() { return cv::Rect(x, y, w, h); }

	FaceBox(int x_, int y_, int w_, int h_, float a_, float s_, float c_)
		: x(x_), y(y_), w(w_), h(h_), angle(a_), scale(s_), score(c_)
	{
	}
};
