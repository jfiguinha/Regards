#pragma once
#include "EffectParameter.h"

class Chqdn3dEffectParameter : public CEffectParameter
{
public:
	Chqdn3dEffectParameter(): width(0), height(0)
	{
		LumSpac = 4;
		ChromSpac = 4;
		LumTmp = 3;
		ChromTmp = 3;
	}
	;

	~Chqdn3dEffectParameter() override
	{
	};

	double LumSpac;
	double ChromSpac;
	double LumTmp;
	double ChromTmp;
	int width;
	int height;
};
