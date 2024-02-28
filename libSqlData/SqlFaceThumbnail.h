#pragma once
#include <ThumbnailData.h>
class CRegardsBitmap;
class CImageLoadingFormat;

class CSqlFaceThumbnail : public CThumbnailData
{
public:
	CSqlFaceThumbnail(const wxString& filename, const int& numFace);
	~CSqlFaceThumbnail(void) override;

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

	cv::Mat GetwxImage(bool& isDefault) override;

private:
	bool TestBitmap() override;
	int numFace;
};
