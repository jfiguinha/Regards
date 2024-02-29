#pragma once
#include "ThumbnailData.h"
class CImageLoadingFormat;

class CThumbnailDataStorage : public CThumbnailData
{
public:
	CThumbnailDataStorage(const wxString& filename);
	~CThumbnailDataStorage(void) override;

	int GetType() override
	{
		return 1;
	}

	void RemoveBitmap() override;
	void SetBitmap(cv::Mat bitmap) override;
	cv::Mat GetImage(bool& defaultValue) override;
    bool GetIsDefault();
    void SetIsDefault(const bool & defaultValue);

private:
	bool TestBitmap() override;
	cv::Mat localbitmap;
    bool isDefault = true;
};
