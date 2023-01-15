#pragma once
#include <StreamInfo.h>

namespace Regards::Video
{
	class IVideoInterface
	{
	public:
		virtual ~IVideoInterface() = default;
		virtual void OnVideoEnd() = 0;
		virtual void OnVideoStart() = 0;
		virtual void OnVideoStop() = 0;
		virtual void OnVideoPause() = 0;
		virtual void OnAfterOpenVideo() = 0;
		virtual void OnPositionVideo(const int64_t& position) = 0;
		virtual void SetVideoDuration(const int64_t& position) = 0;
		virtual void SetStreamInfo(vector<CStreamInfo>& listAudio, vector<CStreamInfo>& listVideo,
		                           vector<CStreamInfo>& listSubtitle) = 0;
	};
}
