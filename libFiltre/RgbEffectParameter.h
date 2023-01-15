#pragma once
#include "EffectParameter.h"

class CRgbEffectParameter : public CEffectParameter
{
public:
	CRgbEffectParameter()
	{
		red = 120;
		green = 120;
		blue = 120;
	};

	~CRgbEffectParameter() override
	{
	};

	int red;
	int green;
	int blue;
};
