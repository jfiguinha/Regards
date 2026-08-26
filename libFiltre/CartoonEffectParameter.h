#pragma once
#include "EffectParameter.h"

class CCartoonEffectParameter : public CEffectParameter
{
public:
	CCartoonEffectParameter()
	{
		mode = 0;
	};


	int mode = 0;
};
