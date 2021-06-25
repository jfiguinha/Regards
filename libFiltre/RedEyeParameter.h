#pragma once
#include "EffectParameter.h"
class CRedEyeFiltreEffectParameter : public CEffectParameter
{
public:
	CRedEyeFiltreEffectParameter()
	{
		intensity = 0.8f;
	}

	~CRedEyeFiltreEffectParameter() override = default;

	float intensity;
};

