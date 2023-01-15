#pragma once
#include "EffectParameter.h"

class CBlurEffectParameter : public CEffectParameter
{
public:
	CBlurEffectParameter()
	{
		size = 3;
	};

	~CBlurEffectParameter() override
	{
	};

	int size;
};
