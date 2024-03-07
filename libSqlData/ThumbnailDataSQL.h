#pragma once
#include "ThumbnailData.h"

class CImageLoadingFormat;

namespace Regards
{
	namespace Video
	{
		class IVideoPlayer;
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

	cv::Mat GetImage(bool& isDefault) override;
	int GetNbFrame() override;

private:
	bool TestBitmap() override;
	int nbFrame = 0;
	cv::Mat cvImg;
	int time_pos = 0;
	cv::Mat frameOut;
    bool defaultPicture = true;
	int oldnumFrame = -1;
	int oldVideoFrame = 0;
	int videoFramePos = 0;
	bool mouseOn = false;
    bool generateVideoPlayer = false;
    bool useOpenCV = false;
    Regards::Video::IVideoPlayer * videoCaptureCV = nullptr;

};
