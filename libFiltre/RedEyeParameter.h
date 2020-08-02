#include "EffectParameter.h"
#pragma once

class CRedEyeFiltreEffectParameter : public CEffectParameter
{
public:
	CRedEyeFiltreEffectParameter()
	{
		intensity = 0.8f;
	};
	~CRedEyeFiltreEffectParameter(){};

	float intensity;
};

