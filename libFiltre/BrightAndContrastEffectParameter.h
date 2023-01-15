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

	~CBrightAndContrastEffectParameter() override
	{
	};

	long contrast;
	long brightness;
};
