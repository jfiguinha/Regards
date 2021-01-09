#include "EffectParameter.h"
#pragma once

class Chqdn3dEffectParameter : public CEffectParameter
{
public:
	Chqdn3dEffectParameter()
	{
		LumSpac = 4;
		ChromSpac = 4;
		LumTmp = 3;
		ChromTmp = 3;
	};
	~Chqdn3dEffectParameter(){};

	double LumSpac;
	double ChromSpac;
	double LumTmp;
	double ChromTmp;
	int width;
	int height;
};

