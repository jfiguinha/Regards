#pragma once
#include "EffectParameter.h"
class CSolarisationEffectParameter : public CEffectParameter
{
public:
	CSolarisationEffectParameter()
	{
		threshold = 127;
	};
	~CSolarisationEffectParameter() override {};

	long threshold;
};

