#pragma once
#include "EffectParameter.h"

class CFreeRotateEffectParameter : public CEffectParameter
{
public:
	CFreeRotateEffectParameter()
	{
		angle = 0;
	};


	int angle;
};
