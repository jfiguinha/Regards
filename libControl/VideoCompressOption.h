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
};