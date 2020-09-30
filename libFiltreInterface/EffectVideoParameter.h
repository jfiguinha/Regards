#pragma once
#include "EffectParameter.h"
//#include <ParamInit.h>
//#include <RegardsConfigParam.h>

class CVideoEffectParameter : public CEffectParameter
{
public:
	CVideoEffectParameter()
	{
		effectEnable = 0;
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
        enableOpenCL = 0;
		denoiseEnable = 0;
        bandcEnable = 1;

		uSigma = 11.0;
		uThreshold = 18;
		uKSigma = 2;

		filmgrainenable = 0;
		/*
		GLfloat sigma = 11.0f, threshold = .180f, slider = 0.f; //running
		// GLfloat sigma = 7.0f, threshold = .180f, slider = 0.f; //running
		GLfloat kSigma = 2.f;

		GLfloat aSigma[3] = {9.f, 7.f, 7.f};
		GLfloat aThreshold[3] = {.180f, .195f, .180f};
		GLfloat aKSigma[3] = {3.f, 3.f, 2.f};
		*/
	};
	~CVideoEffectParameter()
	{
	};

	//Video Denoising Parameter
	float uSigma;
	float uThreshold;
	float uKSigma;
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
	int	  filmgrainenable;	
};

