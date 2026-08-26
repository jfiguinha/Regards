#pragma once
#include "EffectParameter.h"

class COilPaintingEffectParameter : public CEffectParameter
{
public:
	COilPaintingEffectParameter()
	{
		size = 10;
		dynRatio = 1;
	};

	int size;
	int dynRatio;
};
