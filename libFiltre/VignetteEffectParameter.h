#pragma once

class CVignetteEffectParameter : public CEffectParameter
{
public:
	CVignetteEffectParameter()
	{
		radius = 1.0;
		bitmapWidth = 0;
		bitmapHeight = 0;
		power = 0.8;
	};
	~CVignetteEffectParameter() {};

	float radius;
	float power;
	int bitmapWidth;
	int bitmapHeight;
};