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

	void RemoveBitmap();
	void SetBitmap(CImageLoadingFormat* bitmap) override;
	wxImage GetwxImage() override;

private:
	bool TestBitmap() override;
	wxImage* localbitmap;
};
