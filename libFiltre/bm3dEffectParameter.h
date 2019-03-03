#include "EffectParameter.h"
#pragma once

class CBm3dEffectParameter : public CEffectParameter
{
public:
	CBm3dEffectParameter()
	{
		fSize = 25;
	};
	~CBm3dEffectParameter(){};

	int fSize;
};