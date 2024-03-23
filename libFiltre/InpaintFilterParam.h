#pragma once

#pragma once
#include "EffectParameter.h"

class CInpaintFilterParameter : public CEffectParameter
{
public:
	CInpaintFilterParameter()
	{
		algo = 0; //RECURS_FILTER 
	};

	~CInpaintFilterParameter() override
	{
	};

	int algo;
};

