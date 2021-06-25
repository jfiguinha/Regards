#pragma once
#include <EffectVideoParameter.h>

class CVideoOptionCompress
{
public:
	CVideoOptionCompress(): audioBitRate(0), audioQuality(0), audioQualityOrBitRate(false), videoBitRate(0),
	                        videoHardware(false),
	                        constantOrVbrOption(0),
	                        videoCompressionValue(0),
	                        videoQualityOrBitRate(false)
	{
	}

	~CVideoOptionCompress()
	{
	}

	//Audio
	int audioBitRate;
	int audioQuality;
	wxString audioCodec;
	bool audioQualityOrBitRate;
	long startTime = 0;
	long endTime = 0;
	//Video
	int videoBitRate;
	bool videoHardware;
	wxString videoCodec;
	wxString videoPreset;
	int constantOrVbrOption;
	int videoCompressionValue;
	bool videoQualityOrBitRate;
	wxString encoder_profile = "main";
	wxString encoder_level = "";
	bool audioDirectCopy = false;
	bool videoDirectCopy = false;
	bool interlace = false;
	CVideoEffectParameter videoEffectParameter;
};
