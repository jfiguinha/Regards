#pragma once
#include "EffectParameter.h"

class CDecodeRawParameter : public CEffectParameter
{
public:
	CDecodeRawParameter()
	{
		highlight = 0;
		bright = 2.0;
		threshold = 0;
		use_auto_wb = 0;
		use_camera_wb = 0;
	};
	~CDecodeRawParameter() {};

	float bright; //Brightness (default 1.0).
	int highlight; //0-9: Highlight mode (0=clip, 1=unclip, 2=blend, 3+=rebuild).
	int threshold;
	int use_auto_wb;
	int use_camera_wb;
	bool update = false;
};

