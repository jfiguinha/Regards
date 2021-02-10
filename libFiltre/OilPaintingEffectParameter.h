#include "EffectParameter.h"
#pragma once

class COilPaintingEffectParameter : public CEffectParameter
{
public:
	COilPaintingEffectParameter()
	{
		size = 10;
		dynRatio = 1;
	};
	~COilPaintingEffectParameter() {};

	int size;
	int dynRatio;

};

