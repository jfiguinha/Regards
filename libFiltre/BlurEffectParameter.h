#include "EffectParameter.h"
#pragma once

class CBlurEffectParameter : public CEffectParameter
{
public:
	CBlurEffectParameter()
	{
		size = 3;
	};
	~CBlurEffectParameter(){};

	int size;
};

