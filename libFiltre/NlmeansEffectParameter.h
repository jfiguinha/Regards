#include "EffectParameter.h"
#pragma once

class CNlmeansEffectParameter : public CEffectParameter
{
public:
	CNlmeansEffectParameter()
	{
		fSize = 1;
		bSize = 1;
		sigma = 1.0f;
	};
	~CNlmeansEffectParameter(){};

	int fSize;
	int bSize;
	float sigma;

};

