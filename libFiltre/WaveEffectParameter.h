#include "EffectParameter.h"
#pragma once

class CWaveEffectParameter : public CEffectParameter
{
public:
	CWaveEffectParameter()
	{
		height = 200;
		radius = 20;
		scale = 2;
	};
	~CWaveEffectParameter(){};

	short height;
	int radius;
	int scale;
};


