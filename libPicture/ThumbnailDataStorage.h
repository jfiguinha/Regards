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
	void SetBitmap(wxImage bitmap) override;
	wxImage GetwxImage(bool& isDefault) override;
    bool GetIsDefault();
    void SetIsDefault(const bool &isDefault);

private:
	bool TestBitmap() override;
	wxImage localbitmap;
    bool isDefault = true;
};
