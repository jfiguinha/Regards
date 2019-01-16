#pragma once
#include "ThumbnailData.h"
class CImageLoadingFormat;

class CThumbnailDataStorage : public CThumbnailData
{
public:
	CThumbnailDataStorage(const wxString & filename);
	~CThumbnailDataStorage(void);

	int GetType()
	{
		return 1;
	}

	void SetBitmap(CImageLoadingFormat * bitmap);
	wxImage GetwxImage();

private:

	bool TestBitmap();
	uint8_t * data;
	unsigned long size;
	int format;
	int compressMethod;
};

