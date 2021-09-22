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
		use_auto_wb = false;
		use_camera_wb = false;
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
		//Need to implement
		use_fuji_rotate = false;
		cfa_clean = false;
		cfa_green = false;
		exp_correc = false;
		wf_debanding = 0;
		adjust_maximum_thr = 0.75;
		use_fuji_rotate = -1;
		green_matching = 0;
		dcb_iterations = -1;
		dcb_enhance_fl = 0;
		fbdd_noiserd = 0;
		eeci_refine = 0;
		es_med_passes = 0;
		ca_correc = 0;
		cared = 0;
		cablue = 0;
		cfaline = 0;
		linenoise = 0.0;
		cfa_clean = 0;
		// lclean = 0,01;
		cclean = 0.01;
		cfa_green = 0;
		green_thresh = 0.01;
		exp_correc = 0;
		exp_shift = 1.0;
		exp_preser = 0.0;
		wf_debanding = 0;
		wf_deband_tresholdRed = 0;
		wf_deband_tresholdGreen = 0;
		wf_deband_tresholdBlue = 0;
		wf_deband_tresholdOther = 0;
	};
	~CDecodeRawParameter() {};

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
	int eeci_refine;
	int es_med_passes;
	int ca_correc;
	float cared;
	float cablue;
	int cfaline;
	float linenoise;
	bool cfa_clean;
	float lclean, cclean;
	bool cfa_green;
	float green_thresh;
	bool exp_correc;
	float exp_shift, exp_preser;
	bool wf_debanding;
	float wf_deband_tresholdRed;
	float wf_deband_tresholdGreen;
	float wf_deband_tresholdBlue;
	float wf_deband_tresholdOther;
};

