#pragma once
#include "EffectParameter.h"
#include <ParamInit.h>
#include <RegardsConfigParam.h>

class CVideoEffectParameter : public CEffectParameter
{
public:
	CVideoEffectParameter()
	{
		int supportOpenCL = 0;
		CRegardsConfigParam* config = CParamInit::getInstance();
		if (config != nullptr)
			supportOpenCL = config->GetIsOpenCLSupport();

		effectEnable = 0;
		//Median value
		bSize = 3;
		fSize = 3;
		sigmaX = 8.0;
		sigmaP = 8.0;
		//Video Parameter
		sharpness = 1.5f;
		//Sky Light
		//Tone mapping
		contrast = 0.0f;
		brightness = 0.0f;
		exponent = 1.0f;

		//Color Boost
		color_boost[0] = 0.0f;
		color_boost[1] = 0.0f;
		color_boost[2] = 0.0f;
		color_boost[3] = 0.0f;

		SharpenEnable = 1;
		MedianEnable = 1;
		ColorBoostEnable = 1;
		BicubicEnable = 0;
		rotation = 0;
		showFPS = 0;
		grayEnable = 0;
		sepiaEnable = 0;
		enableSubtitle = 0;
		streamAudioUpdate = 0;
		streamVideoUpdate = 0;
		streamSubtitleUpdate = 0;
        enableOpenCL = supportOpenCL;
		denoiseEnable = 0;
        bandcEnable = 1;
	};
	~CVideoEffectParameter()
	{
	};

	//Video Denoising Parameter
	int bSize;
	int fSize;
	float sigmaP;
	float sigmaX;
	float exponent;

	//Video Parameter
	float sharpness;

	//Tone mapping
	float contrast;
	float brightness;

	//Color Boost
	float color_boost[4];

	int	  SharpenEnable;
	int   MedianEnable;
	int	  denoiseEnable;
	int	  ColorBoostEnable;
	int	  BicubicEnable;
    int   enableOpenCL;
	int	  rotation;
	int	  bandcEnable;
	int	  showFPS;
	int   effectEnable;
	int	  grayEnable;
	int	  sepiaEnable;
	int   enableSubtitle;
	int   streamAudioIndex;
	int   streamVideoIndex;
	int   streamSubtitleIndex;
	int   streamAudioUpdate;
	int   streamVideoUpdate;
	int   streamSubtitleUpdate;
};

