#pragma once
#include "EffectParameter.h"
class CPosterisationEffectParameter : public CEffectParameter
{
public:
	CPosterisationEffectParameter()
	{
		level = 4;
		gamma = 0.6;
	};
	~CPosterisationEffectParameter() override {};

	long level;
	float gamma;
};

