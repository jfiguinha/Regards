#pragma once
#include "EffectParameter.h"

class CBm3dEffectParameter : public CEffectParameter
{
public:
	CBm3dEffectParameter()
	{
		fSize = 5;
	};

	~CBm3dEffectParameter() override
	{
	};

	int fSize;
};
