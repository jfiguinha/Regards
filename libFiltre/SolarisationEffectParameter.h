#pragma once
#include "EffectParameter.h"

class CSolarisationEffectParameter : public CEffectParameter
{
public:
	CSolarisationEffectParameter()
	{
		threshold = 50;
	};

	long threshold;
};
