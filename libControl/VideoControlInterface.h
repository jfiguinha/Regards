#pragma once

extern "C" {
#include "libavcodec/avcodec.h"
}
#define PAUSE 1
#define PLAY 2
#define STOP 3
#define EVENT_VIDEOSTART 1001
#define EVENT_VIDEOSTOP 1003
#define EVENT_REFRESHSCREEN 1002


class DataTimeDuration
{
public:
	int64_t duration;
	int64_t startTime;
};

class CVideoControlInterface
{
public:

	virtual ~CVideoControlInterface() = default;
	virtual wxWindow * GetMainWindow() = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
};
