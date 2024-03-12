#pragma once
#include "EffectParameter.h"

class CStylizationEffectParameter : public CEffectParameter
{
public:
	CStylizationEffectParameter()
	{
		sigmaX = 60;
		sigmaP = 45;
	};

	~CStylizationEffectParameter() override
	{
	};

	int sigmaX;
	int sigmaP;
};