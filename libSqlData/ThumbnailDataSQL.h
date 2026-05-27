#pragma once
#include "ThumbnailData.h"
#include <memory>

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
	CThumbnailDataSQL(const wxString& filename, const bool& testValidity, const bool& generateVideoPlayer);
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

	// FIX [mineur #8, #9, #11] : suppression des membres morts
	// time_pos, cvImg et oldnumFrame retirés car jamais lus ni écrits.

	int nbFrame = 0;
	cv::Mat frameOut;
	bool defaultPicture = true;

	// FIX [sérieux #5] : oldVideoFrame conservé mais son usage est désormais
	// implémenté dans SetMouseOn pour reprendre la lecture à la bonne position.
	int oldVideoFrame = 0;
	int videoFramePos = 0;
	bool mouseOn = false;
	bool generateVideoPlayer = false;

	// FIX [critique #2] : unique_ptr garantit la destruction et évite le delete
	// manuel tout en restant compatible avec le polymorphisme IVideoPlayer.
	std::unique_ptr<Regards::Video::IVideoPlayer> videoCaptureCV;
};