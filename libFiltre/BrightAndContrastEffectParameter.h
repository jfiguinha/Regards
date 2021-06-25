#pragma once
#include "EffectParameter.h"
class CBrightAndContrastEffectParameter : public CEffectParameter
{
public:
	CBrightAndContrastEffectParameter()
	{
		contrast = 0;
		brightness = 0;
	};
	~CBrightAndContrastEffectParameter(){};

	long contrast;
	long brightness;
};

