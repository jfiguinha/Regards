#pragma once

class CVignetteEffectParameter : public CEffectParameter
{
public:
	CVignetteEffectParameter()
	{
		radius = 50;
		bitmapWidth = 0;
		bitmapHeight = 0;
		power = 50;
	};
	~CVignetteEffectParameter() {};

	float radius;
	float power;
	int bitmapWidth;
	int bitmapHeight;
};