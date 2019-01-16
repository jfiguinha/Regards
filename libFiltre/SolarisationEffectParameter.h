#include "EffectParameter.h"
#pragma once

class CSolarisationEffectParameter : public CEffectParameter
{
public:
	CSolarisationEffectParameter()
	{
		threshold = 127;
	};
	~CSolarisationEffectParameter(){};

	long threshold;
};

