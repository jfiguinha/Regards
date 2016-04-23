#include "EffectParameter.h"
#pragma once

class CFreeRotateEffectParameter : public CEffectParameter
{
public:
	CFreeRotateEffectParameter()
	{
		angle = 0;
	};
	~CFreeRotateEffectParameter(){};

	int angle;
};

