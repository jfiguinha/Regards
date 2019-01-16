#pragma once
#include <ThumbnailData.h>
class CRegardsBitmap;
class CImageLoadingFormat;

class CSqlFaceThumbnail : public CThumbnailData
{
public:
	CSqlFaceThumbnail(const wxString & filename,const int & numFace);
	~CSqlFaceThumbnail(void);

	int GetType()
	{
		return 2;
	}

	bool IsBuffer()
	{
		return false;
	}

	int GetNumFace()
	{
		return numFace;
	}

	void SetBitmap(CImageLoadingFormat * bitmap);
	wxImage GetwxImage();

private:

	bool TestBitmap();
	int numFace;

};
