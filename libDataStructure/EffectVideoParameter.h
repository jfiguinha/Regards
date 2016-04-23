#pragma once

class CVideoEffectParameter
{
public:
	CVideoEffectParameter()
	{
		//Median value
		median = 1.0f;
		//Video Parameter
		sharpness = 1.5f;
		//Sky Light
		skyColor[0] = 0.1f;
		skyColor[1] = 0.1f;
		skyColor[2] = 0.5f;
		skyColor[3] = 1.0f;

		baseColor[0] = 0.0f;
		baseColor[1] = 0.0f;
		baseColor[2] = 0.1f;
		baseColor[3] = 1.0f;

		groundColor[0] = 0.1f;
		groundColor[1] = 0.4f;
		groundColor[2] = 0.1f;
		groundColor[3] = 1.0f;

		skylight_amount = 0.5f;

		//Tone mapping
		contrast = 1.5f;
		exposure = 1.5f;
		brightness = 2.25f;

		//Color Boost
		color_boost[0] = 1.0f;
		color_boost[1] = 1.0f;
		color_boost[2] = 1.0f;
		color_boost[3] = 1.0f;

		//Compliment
		guide[0] = 1.0f;
		guide[1] = 0.79f;
		guide[2] = 0.43f;
		guide[3] = 1.0f;

		amount = 0.5f;
		correlation = 0.5f;
		concentration = 1.5f;

		SharpenEnable = 1;
		SkyLightEnable = 1;
		ToneEnable = 0;
		ColorEnable = 1;
		ComplimentEnable = 1;
		MedianEnable = 1;
		ColorBoostEnable = 0;
		BicubicEnable = 1;
		rotation = 0;
	};
	~CVideoEffectParameter()
	{
	};

	//Video Parameter
	float median;

	//Video Parameter
	float sharpness;
	//Sky Light
	float skyColor[4];
	float baseColor[4];
	float groundColor[4];
	float skylight_amount;

	//Tone mapping
	float contrast;
	float exposure;
	float brightness;

	//Color Boost
	float color_boost[4];

	//Compliment
	float guide[4];
	float amount;
	float correlation;
	float concentration;

	int	  SharpenEnable;
	int	  SkyLightEnable;
	int	  ToneEnable;
	int	  ColorEnable;
	int	  ComplimentEnable;
	int   MedianEnable;
	int	  ColorBoostEnable;
	int	  BicubicEnable;
	int	  rotation;

	int effectEnable = 0;
};

