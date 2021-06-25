#pragma once
#include "EffectParameter.h"
class CNlmeansEffectParameter : public CEffectParameter
{
public:
	CNlmeansEffectParameter()
	{
		fSize = 1;
		bSize = 1;
		sigma = 1.0f;
	};
	~CNlmeansEffectParameter() override {};

	int fSize;
	int bSize;
	float sigma;

};

