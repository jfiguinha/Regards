#pragma once
#include "EffectParameter.h"
class CSwirlEffectParameter : public CEffectParameter
{
public:
	CSwirlEffectParameter()
	{
		radius = 0;
		bitmapWidth = 0;
		bitmapHeight = 0;
		angle = 0;
	};
	~CSwirlEffectParameter() override {};

	int radius;
	int angle;
	int bitmapWidth;
	int bitmapHeight;
};

