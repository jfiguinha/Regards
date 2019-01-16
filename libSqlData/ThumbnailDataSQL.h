#pragma once
#include <ThumbnailData.h>
#include <ImageVideoThumbnail.h>

class CImageLoadingFormat;

struct VideoParameter
{
	wxString filename;
	vector<CImageVideoThumbnail *> * listVideo;
};

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
