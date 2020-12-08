#pragma once

class CVideoOptionCompress
{
public:

	CVideoOptionCompress()
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
};