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
	~COilPaintingEffectParameter() override {};

	int size;
	int dynRatio;

};

