#pragma once
#include "EffectParameter.h"

class CDetailEnhanceParameter : public CEffectParameter
{
public:
	CDetailEnhanceParameter()
	{
		sigmaX = 10;
		sigmaP = 15;
	};

	~CDetailEnhanceParameter() override
	{
	};

	int sigmaX;
	int sigmaP;
};
