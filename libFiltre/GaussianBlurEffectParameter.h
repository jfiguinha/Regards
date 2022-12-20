
#pragma once
#include "EffectParameter.h"
class CGaussianBlurEffectParameter : public CEffectParameter
{
public:
	CGaussianBlurEffectParameter()
	{
		radius = 5;
		boxSize = 3;
	};
	~CGaussianBlurEffectParameter(){};

	int radius;
	int boxSize;
};

