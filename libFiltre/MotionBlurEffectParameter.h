#pragma once
#include "EffectParameter.h"
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
	~CMotionBlurEffectParameter() override {};

	int radius;
	int sigma;
	int angle;
	int bitmapWidth;
};

