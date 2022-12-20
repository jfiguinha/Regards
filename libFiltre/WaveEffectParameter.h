#pragma once
#include "EffectParameter.h"
class CWaveEffectParameter : public CEffectParameter
{
public:
	CWaveEffectParameter()
	{
		height = 200;
		radius = 20;
		scale = 2;
	};
	~CWaveEffectParameter() override {};

	short height;
	int radius;
	int scale;
};


