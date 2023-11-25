#pragma once
#include "EffectParameter.h"

class CBitmapFusionEffectParameter : public CEffectParameter
{
public:
	CBitmapFusionEffectParameter()
	{
		bmpSecond = nullptr;
		etape = 0;
	};

	~CBitmapFusionEffectParameter() override
	{
	};

	CRegardsBitmap* bmpSecond;
	int etape;
};
