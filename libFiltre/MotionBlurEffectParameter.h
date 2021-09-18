#pragma once
#include "EffectParameter.h"
class CMotionBlurEffectParameter : public CEffectParameter
{
public:
	CMotionBlurEffectParameter()
	{
		radius = 20;
		sigma = 5;
		angle = 40;
		bitmapWidth = 0;
	};
	~CMotionBlurEffectParameter() override {};

	int radius;
	int sigma;
	int angle;
	int bitmapWidth;
};

