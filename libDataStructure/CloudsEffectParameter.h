#pragma once
#include "EffectParameter.h"

class CCloudsEffectParameter : public CEffectParameter
{
public:
	CCloudsEffectParameter()
	{
		colorFront = CRgbaquad(0, 0, 0);
		colorBack = CRgbaquad(255, 255, 255);

		amplitude = 0.5;
		frequence = 0.5;
		octave = 8;
	};
	~CCloudsEffectParameter(){};

	CRgbaquad colorFront;
	CRgbaquad colorBack;

	int octave;
	float amplitude;
	float frequence;
};

