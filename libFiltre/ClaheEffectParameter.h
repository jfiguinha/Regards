#include "EffectParameter.h"
#pragma once

class CClaheEffectParameter : public CEffectParameter
{
public:
	CClaheEffectParameter()
	{
		windowSize = 150;
		nBins = 128;
		clipLevel = 3.0f;
	};
	~CClaheEffectParameter(){};

	int nBins;
	float clipLevel;
	int windowSize;
};

