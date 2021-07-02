#pragma once

class CBokehEffectParameter : public CEffectParameter
{
public:
	int blurvalue = 19;
	int bokehthreshold = 253;
	int bokehthreshold2 = 248;
	int dilation_size = 10;
	int dilation_size2 = 7;
};