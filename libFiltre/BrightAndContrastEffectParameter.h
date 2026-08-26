#pragma once
#include "EffectParameter.h"

class CBrightAndContrastEffectParameter : public CEffectParameter
{
public:
	CBrightAndContrastEffectParameter()
	{
		contrast = 15;
		brightness = 20;
	};

	long contrast;
	long brightness;
};
