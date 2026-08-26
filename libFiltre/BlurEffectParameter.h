#pragma once
#include "EffectParameter.h"

class CBlurEffectParameter : public CEffectParameter
{
public:
	CBlurEffectParameter()
	{
		size = 3;
	};

	int size;
};
