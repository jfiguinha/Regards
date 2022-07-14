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
	CThumbnailDataSQL(const wxString& filename, const bool& testValidity);
	~CThumbnailDataSQL(void) override;

	int GetType() override
	{
		return 2;
	}

	bool IsBuffer()
	{
		return false;
	}

	void SetBitmap(CImageLoadingFormat* bitmap) override;
	wxImage GetwxImage() override;
	int GetNbFrame() override;

private:
	bool TestBitmap() override;
	int nbFrame = 0;
	bool isVideo = false;
	cv::Mat cvImg;
	int time_pos = 0;
	wxImage frameOut;
	int oldnumFrame = -1;
	bool useOpenCV = false;
	Regards::Video::CVideoPlayer * videoCapture = nullptr;
	cv::VideoCapture* videoCaptureCV = nullptr;
};
