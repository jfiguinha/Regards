#pragma once

typedef void(*SetData)(void*, void*, float);
typedef void(*SetPos)(void*, int64_t);
typedef void(*SetVideoDuration)(void*, int64_t, int64_t);
typedef void(*VideoStart)(void*, int64_t, int64_t);
typedef void(*VideoEnd)(void*);
/*
	virtual void SetPos(int64_t pos) = 0;
	virtual void SetData(void* data, const float& sample_aspect_ratio, void* WIN32Context) = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
    virtual void ErrorDecodingFrame() = 0;
    virtual wxString GetAcceleratorHardware() = 0;
	virtual void SetSubtitulePicture(cv::Mat& picture) = 0;
	virtual void DeleteSubtitulePicture() = 0;

	virtual int ChangeAudioStream(int newStreamAudio) = 0;
	//virtual void VolumeUp() = 0;
	//virtual void VolumeDown() = 0;
	virtual int GetVolume() = 0;

	virtual void SetRotation(const int& rotation) = 0;
	virtual void SetVideoDuration(const int64_t& duration, const int64_t& startTime) = 0;
	//virtual void SetVideoPosition(const int64_t & pos) = 0;
*/

typedef struct ListFunction
{
    SetData setData;
    SetPos setPos;
	VideoStart videoStart;
    SetVideoDuration setVideoDuration;
	VideoEnd videoEnd;
    void * wxWindowParent;
}ListFunction;
