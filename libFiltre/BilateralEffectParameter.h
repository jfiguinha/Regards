#pragma once
#include "EffectParameter.h"
class CBilateralEffectParameter : public CEffectParameter
{
public:
	CBilateralEffectParameter()
	{
		fSize = 3;
		sigmaX = 1.0f;
		sigmaP = 1.0f;
	};
	~CBilateralEffectParameter() override {};

	int fSize;
	float sigmaX;
	float sigmaP;
};

