#pragma once
#include <ThumbnailData.h>
#include <RegardsBitmap.h>
#include <stdint.h>
class CImageLoadingFormat;

class CThumbnailDataSQL : public CThumbnailData
{
public:
	CThumbnailDataSQL(const wxString & filename, const bool &testValidity);
	~CThumbnailDataSQL(void);

	int GetType()
	{
		return 2;
	}

	bool IsBuffer()
	{
		return false;
	}

	void SetBitmap(CImageLoadingFormat * bitmap);
	wxImage GetwxImage();

private:

	bool TestBitmap();

};
