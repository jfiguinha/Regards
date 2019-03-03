#include "EffectParameter.h"
#pragma once

class CPosterisationEffectParameter : public CEffectParameter
{
public:
	CPosterisationEffectParameter()
	{
		level = 4;
		gamma = 0.6;
	};
	~CPosterisationEffectParameter(){};

	long level;
	float gamma;
};

