#include "EffectParameter.h"
#pragma once

class CMotionBlurEffectParameter : public CEffectParameter
{
public:
	CMotionBlurEffectParameter()
	{
		radius = 0;
		sigma = 0;
		angle = 0;
		bitmapWidth = 0;
	};
	~CMotionBlurEffectParameter(){};

	int radius;
	int sigma;
	int angle;
	int bitmapWidth;
};

