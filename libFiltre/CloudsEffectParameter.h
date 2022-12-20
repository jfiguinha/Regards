#pragma once
#include "EffectParameter.h"
#include <RGBAQuad.h>

class CCloudsEffectParameter : public CEffectParameter
{
public:
	CCloudsEffectParameter()
	{
		colorFront = CRgbaquad(0, 0, 0);
		colorBack = CRgbaquad(255, 255, 255);

		transparency = 0;
		amplitude = 1;
		frequence = 65;
		octave = 8;
	};
	~CCloudsEffectParameter(){};

	CRgbaquad colorFront;
	CRgbaquad colorBack;

	int octave;
	int amplitude;
	int frequence;
	int transparency;
};

