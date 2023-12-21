#pragma once
#include "ThumbnailData.h"

class CImageLoadingFormat;

namespace Regards
{
	namespace Video
	{
		class CVideoPlayer;
	}
}


class CThumbnailDataSQL : public CThumbnailData
{
public:
	CThumbnailDataSQL(const wxString& filename, const bool& testValidity, const bool & generateVideoPlayer);
	~CThumbnailDataSQL(void) override;

	int GetType() override
	{
		return 2;
	}

	bool IsBuffer()
	{
		return false;
	}

	void SetMouseOn();
	void SetMouseOut();

	wxImage GetwxImage() override;
	int GetNbFrame() override;

private:
	bool TestBitmap() override;
	int nbFrame = 0;
	bool isVideo = false;
	bool isAnimation = false;
	cv::Mat cvImg;
	int time_pos = 0;
	wxImage frameOut;
	int oldnumFrame = -1;
	int oldVideoFrame = 0;
	int videoFramePos = 0;
	bool mouseOn = false;
    bool generateVideoPlayer = false;
	Regards::Video::CVideoPlayer * videoCaptureCV = nullptr;
};
