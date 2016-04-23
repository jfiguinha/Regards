#include "EffectParameter.h"
#pragma once

class CLensFlareEffectParameter : public CEffectParameter
{
public:
	CLensFlareEffectParameter()
	{
		size = 10;
		brightness = 25;
		colorIntensity = 10;
		color = 20;
	};
	~CLensFlareEffectParameter(){};

	int brightness;
	int colorIntensity;
	int size;
	int color;
};

