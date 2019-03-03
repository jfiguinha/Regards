#include "EffectParameter.h"
#pragma once

class CBilateralEffectParameter : public CEffectParameter
{
public:
	CBilateralEffectParameter()
	{
		fSize = 3;
		sigmaX = 1.0f;
		sigmaP = 1.0f;
	};
	~CBilateralEffectParameter(){};

	int fSize;
	float sigmaX;
	float sigmaP;
};

