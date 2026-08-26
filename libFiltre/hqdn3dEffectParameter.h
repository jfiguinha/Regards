#pragma once
#include "EffectParameter.h"

class Chqdn3dEffectParameter : public CEffectParameter
{
public:
	Chqdn3dEffectParameter(): width(0), height(0)
	{

	}

	double LumSpac = 4;
	double temporalLumaDefault = 6.0;
	double temporalSpatialLumaDefault = 4.0;
	int width;
	int height;
};
