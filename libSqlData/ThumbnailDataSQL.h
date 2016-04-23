#pragma once
#include <ThumbnailData.h>
#include <RegardsBitmap.h>
#include <stdint.h>

class CThumbnailDataSQL : public CThumbnailData
{
public:
	CThumbnailDataSQL(const wxString & filename);
	~CThumbnailDataSQL(void);

	int GetType()
	{
		return 2;
	}

	bool IsBuffer()
	{
		return false;
	}

	void SetBitmap(CRegardsBitmap * bitmap);
	wxImage GetwxImage();
	CRegardsBitmap * GetRegardsBitmap(const int &type = 3);

private:

	
	size_t readFileBytes(const wxString &name, uint8_t * data, size_t &size);
	bool TestBitmap();

};
