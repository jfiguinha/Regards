#pragma once
#include "EffectParameter.h"

class CPencilSketchFilterParameter : public CEffectParameter
{
public:
	CPencilSketchFilterParameter()
	{
		shade_factor = 10;
		sigmaX = 60;
		sigmaP = 2;
	};

	~CPencilSketchFilterParameter() override
	{
	};

	int shade_factor;
	int sigmaX;
	int sigmaP;
};

