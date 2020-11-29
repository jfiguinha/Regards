#pragma once

#ifdef WIN32
#include "ffmpeg_dxva2.h"
#else
extern "C"{
#include "libavcodec/avcodec.h"
}
#endif

class CRegardsBitmap;
class CffmpegToBitmap;

#define PAUSE 1
#define PLAY 2
#define STOP 3
#define EVENT_VIDEOSTART 1001
#define EVENT_VIDEOSTOP 1003
#define EVENT_REFRESHSCREEN 1002

class CVideoControlInterface
{
public:
	virtual void SetPos(int64_t pos) = 0;
	virtual void SetData(void * data, const float & sample_aspect_ratio, void * WIN32Context) = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
    
	virtual void SetSubtitulePicture(CRegardsBitmap * picture) = 0;
	virtual void DeleteSubtitulePicture() = 0;
    virtual bool GetDXVA2Compatible() = 0;
    
#ifdef WIN32
	virtual void SetDXVA2Compatible(const bool &compatible) = 0;
	virtual bool GetDXVA2HardwareCompatible() = 0;
	virtual HRESULT InitVideoDevice(char * hwaccel_device, DXVA2Context * ctx, const int &width, const int &height) = 0;
#endif

	virtual int ChangeAudioStream(int newStreamAudio) = 0;
	virtual void VolumeUp() = 0;
	virtual void VolumeDown() = 0;
	virtual int GetVolume() = 0;

	virtual void SetRotation(const int &rotation) = 0;
	virtual void SetVideoDuration(int64_t duration) = 0;
	virtual void SetVideoPosition(int64_t pos) = 0;

};