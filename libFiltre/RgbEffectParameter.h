#pragma once
#include "EffectParameter.h"
class CRgbEffectParameter : public CEffectParameter
{
public:
	CRgbEffectParameter()
	{
		red = 0;
		green = 0;
		blue = 0;
	};
	~CRgbEffectParameter(){};

	int red;
	int green;
	int blue;
};

