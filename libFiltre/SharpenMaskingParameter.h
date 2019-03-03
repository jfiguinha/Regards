#include "EffectParameter.h"
#pragma once

class CSharpenMaskingEffectParameter : public CEffectParameter
{
public:
	CSharpenMaskingEffectParameter()
	{
		sharpness = 1.0f;
	};
	~CSharpenMaskingEffectParameter(){};

	float sharpness;
};

