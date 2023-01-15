#pragma once
#include "EffectParameter.h"

class CLensDistortionEffectParameter : public CEffectParameter
{
public:
	CLensDistortionEffectParameter()
	{
		strength = 10;
	};

	~CLensDistortionEffectParameter() override
	{
	};

	int strength;
};
