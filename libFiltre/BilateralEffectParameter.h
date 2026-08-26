#pragma once
#include "EffectParameter.h"

class CBilateralEffectParameter : public CEffectParameter
{
public:
	CBilateralEffectParameter()
	{
		fSize = 15;
		sigmaX = 80;
		sigmaP = 80;
	};

	int fSize;
	int sigmaX;
	int sigmaP;
};
