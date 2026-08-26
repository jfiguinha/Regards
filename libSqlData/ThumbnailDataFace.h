#pragma once
#include <ThumbnailData.h>
;
class CImageLoadingFormat;

class CThumbnailDataFace : public CThumbnailData
{
public:
	CThumbnailDataFace(const wxString& filename, const int& numFace);
	~CThumbnailDataFace(void) override;

	int GetType() override
	{
		return 4;
	}

	bool IsBuffer()
	{
		return false;
	}

	int GetNumFace()
	{
		return numFace;
	}

	cv::Mat GetImage(bool& isDefault) override;

private:
	bool TestBitmap() override;
	int numFace;
};
