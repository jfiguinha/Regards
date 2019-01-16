#include "EffectParameter.h"
#pragma once

class CRedEyeFiltreEffectParameter : public CEffectParameter
{
public:
	CRedEyeFiltreEffectParameter()
	{
		intensity = 0.8;
	};
	~CRedEyeFiltreEffectParameter(){};

	float intensity;
};

