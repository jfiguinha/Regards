#include "EffectParameter.h"
#pragma once

class CBestExposureEffectParameter : public CEffectParameter
{
public:
	CBestExposureEffectParameter()
	{
		tmo = 2.2f;
	};
	~CBestExposureEffectParameter(){};

	float tmo;
};

