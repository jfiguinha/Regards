#pragma once

class CFFmpegTranscodingPimpl;

class CFFmpegTranscoding
{
public:
	CFFmpegTranscoding();
	~CFFmpegTranscoding();
	int EncodeFile(const char * input, const char * output);

protected:
	CFFmpegTranscodingPimpl * pimpl;

};