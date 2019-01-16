#pragma once
#include <ThumbnailData.h>
#include <ImageVideoThumbnail.h>
#include <stdint.h>
class CImageLoadingFormat;
class CRegardsBitmap;

class CThumbnailDataVideo : public CThumbnailData
{
public:
	CThumbnailDataVideo(const wxString & filename, const bool &testValidity);
	~CThumbnailDataVideo(void);

	int GetType()
	{
		return 3;
	}

	bool IsBuffer()
	{
		return false;
	}

	void SetBitmap(CImageLoadingFormat * bitmap);
	wxImage GetwxImage(const int &numImage);

private:

	vector<CImageVideoThumbnail *> listVideo;

};
