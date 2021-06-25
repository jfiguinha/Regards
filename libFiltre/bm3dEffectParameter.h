#pragma once
#include "EffectParameter.h"
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