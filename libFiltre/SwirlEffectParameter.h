#pragma once
#include "EffectParameter.h"
class CSwirlEffectParameter : public CEffectParameter
{
public:
	CSwirlEffectParameter()
	{
		radius = 20;
		bitmapWidth = 0;
		bitmapHeight = 0;
		angle = 20;
	};
	~CSwirlEffectParameter() override {};

	int radius;
	int angle;
	int bitmapWidth;
	int bitmapHeight;
};

