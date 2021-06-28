#pragma once
#include "EffectParameter.h"
class CNlmeansEffectParameter : public CEffectParameter
{
public:
	CNlmeansEffectParameter()
	{
		h = 3;
		templateWindowSize = 7;
		searchWindowSize = 21;
	};
	~CNlmeansEffectParameter() override {};

	int h;
	int templateWindowSize;
	int searchWindowSize;

};

