#pragma once
#include <ThumbnailData.h>
class CRegardsBitmap;
class CImageLoadingFormat;

class CSqlFaceThumbnail : public CThumbnailData
{
public:
	CSqlFaceThumbnail(const wxString & filename,const int & numFace);
	virtual ~CSqlFaceThumbnail(void);

	int GetType()
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

	wxImage GetwxImage();

private:

	bool TestBitmap();
	int numFace;

};
