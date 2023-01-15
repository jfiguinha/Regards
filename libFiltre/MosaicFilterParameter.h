#pragma once
#include "EffectParameter.h"

class CMosaicEffectParameter : public CEffectParameter
{
public:
	CMosaicEffectParameter()
	{
		size = 5;
	};

	~CMosaicEffectParameter() override
	{
	};

	int size;
};
