
#pragma once
#include "EffectParameter.h"
class CSharpenMaskingEffectParameter : public CEffectParameter
{
public:
	CSharpenMaskingEffectParameter()
	{
		sharpness = 1.0f;
	};
	~CSharpenMaskingEffectParameter() override {};

	float sharpness;
};

