#pragma once

class CFFmpegEncoderPimpl;

class CFFmpegEncoder
{
public:
	CFFmpegEncoder();
	~CFFmpegEncoder();
	int EncodeFile(const char * input, const char * output);

protected:
	CFFmpegEncoderPimpl * pimpl;

};