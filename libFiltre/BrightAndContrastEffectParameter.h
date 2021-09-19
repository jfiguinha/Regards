#pragma once
#include "EffectParameter.h"
class CBrightAndContrastEffectParameter : public CEffectParameter
{
public:
	CBrightAndContrastEffectParameter()
	{
		contrast = 20;
		brightness = 20;
	};
	~CBrightAndContrastEffectParameter(){};

	long contrast;
	long brightness;
};

