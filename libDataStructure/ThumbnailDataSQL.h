#pragma once
#include "ThumbnailData.h"
#include <ImageVideoThumbnail.h>

class CImageLoadingFormat;



class CThumbnailDataSQL : public CThumbnailData
{
public:
	CThumbnailDataSQL(const wxString & filename, const bool &testValidity);
	virtual ~CThumbnailDataSQL(void);

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
    int GetNbFrame();
    
private:
    //int LoadAllThumbnailPictureFromDatabase();
	bool TestBitmap();
    //vector<CImageVideoThumbnail *> listAnimation;
    int nbFrame = 0;
    bool isVideo = false;
   // bool isAnimation = false;
	//VideoParameter param;
};
