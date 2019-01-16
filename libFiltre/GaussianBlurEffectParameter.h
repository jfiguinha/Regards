#include "EffectParameter.h"
#pragma once

class CGaussianBlurEffectParameter : public CEffectParameter
{
public:
	CGaussianBlurEffectParameter()
	{
		radius = 0;
		boxSize = 3;
	};
	~CGaussianBlurEffectParameter(){};

	int radius;
	int boxSize;
};

