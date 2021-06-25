#pragma once
#include "EffectParameter.h"

class CDecodeRawParameter : public CEffectParameter
{
public:
	CDecodeRawParameter()
	{
		highlight = 0;
		bright = 1.0;
		threshold = 1.0;
		use_auto_wb = 1;
		use_camera_wb = 1;
		half_size = 0;
		update = true;
		use_camera_matrix = 1;
		aberGreenEnable = false;
		aberRedEnable = false;
		aberGreen = 0;
		aberRed = 0;
		multiRed = 0;
		multiGreen = 0;
		multiBlue = 0;
		multiOther = 0;
		flip = -1;
		interpolation = 0;
		black = -1;
		blackchannelRed = 0;
		blackchannelGreen = 0;
		blackchannelBlue = 0;
		blackchannelOther = 0;
		saturation = 0;
		medPasses = 0;
		noautobright = false;
		autobright = 0.0;
		exp_correc = false; 
		adjust_maximum_thr =  0.75;
		use_fuji_rotate = false;
		green_matching = 0;
		dcb_iterations = -1;
		dcb_enhance_fl = 0;
		fbdd_noiserd = 0;
		exp_correc = 0; 
		exp_shift = 1.0;
		exp_preser = 0.0;

	};
	~CDecodeRawParameter(){};
	


    
	float bright; //Brightness (default 1.0).
	int highlight; //0-9: Highlight mode (0=clip, 1=unclip, 2=blend, 3+=rebuild).
	float threshold;
	int use_auto_wb;
	int use_camera_wb;
	int half_size;
	int use_camera_matrix;
	bool update;
	int aberGreen;
	int aberRed;
	bool aberGreenEnable;
	bool aberRedEnable;
	int multiRed;
	int multiGreen;
	int multiBlue;
	int multiOther;
	int flip;
	int interpolation;
	int black;
	int blackchannelRed;
	int blackchannelGreen;
	int blackchannelBlue;
	int blackchannelOther;
	int saturation;
	int medPasses;
	bool noautobright;
	float autobright;
	float adjust_maximum_thr;
	bool use_fuji_rotate;
	int green_matching;
	int dcb_iterations;
	int dcb_enhance_fl;
	int fbdd_noiserd;
	bool exp_correc; 
	float exp_shift,exp_preser;

};

