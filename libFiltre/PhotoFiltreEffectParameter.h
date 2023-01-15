#pragma once
#include "EffectParameter.h"

class CPhotoFiltreEffectParameter : public CEffectParameter
{
public:
	CPhotoFiltreEffectParameter()
	{
		red = 0;
		green = 0;
		blue = 0;
		intensity = 0;
	};

	~CPhotoFiltreEffectParameter() override
	{
	};

	int red;
	int green;
	int blue;
	int intensity;
};
