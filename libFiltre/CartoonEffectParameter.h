#include "EffectParameter.h"
#pragma once

class CCartoonEffectParameter : public CEffectParameter
{
public:
	CCartoonEffectParameter()
	{
		mode = 0;
	};
	~CCartoonEffectParameter() {};

	int mode = 0;
};

