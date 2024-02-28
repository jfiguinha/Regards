#pragma once
#include <LoadingResource.h>
#include <libPicture.h>
class CRegardsBitmap;

#define TYPEPHOTO 1
#define TYPECATALOG 2
#define TYPECATEGORIE 3
#define TYPECRITERIA 4
#define TYPEFOLDER 5
#define TYPEVIDEO 6
#define TYPEFACE 7
#define TYPEMULTIPAGE 8
#define SQLITE 1

extern wxImage defaultPictureThumbnailPicture;
extern wxImage defaultPictureThumbnailVideo;

class CImageLoadingFormat;

class CThumbnailData
{
public:
	CThumbnailData(const wxString& filename);
	virtual ~CThumbnailData(void);
	CThumbnailData& operator=(const CThumbnailData& other);

	virtual void RemoveBitmap()
	{
	};

	virtual int GetType()
	{
		return 0;
	};

	void SetNumPhotoId(const int& id);
	int GetNumPhotoId();

	void SetNumElement(const int& iNumElement);
	int GetNumElement();

	wxString GetFilename();
	void SetFilename(const wxString& filename);

	virtual void SetBitmap(cv::Mat bitmap)
	{
	};

	virtual cv::Mat GetImage(bool& isDefault)
	{
		isDefault = true;
		return GetDefaultPicture();
	};

	virtual void SetMouseOn() {};
	virtual void SetMouseOut() {};

	void SetForceRefresh(const bool& refresh);
	bool GetForceRefresh();

	void SetTypeElement(const int& typeElement);
	int GetTypeElement();

	void SetNumCatalog(const int& numCatalog);
	int GetNumCatalog();

	void SetNumCategorie(const int& numCategorie);
	int GetNumCategorie();

	void SetNumParent(const int& numParent);
	int GetNumParent();

	void SetKey(const wxString& key);
	wxString GetKey();

	bool IsLoading();
	void SetIsLoading(const bool& isLoading);

	bool IsProcess();
	void SetIsProcess(const bool& isProcess);

	bool IsLoad();

	void InitLoadState()
	{
		pictureLoad = false;
		pictureLoading = false;
	}

	int GetPercent();
	void SetPercent(const int& percent);

	void SetTimePosition(const int& timePosition);
	int GetTimePosition();
	wxString GetTimePositionLibelle();

	virtual int GetNbFrame()
	{
		return 1;
	}

	virtual int GetNumFrame()
	{
		return numFrame;
	}

	virtual void SetNumFrame(const int& numFrame)
	{
		this->numFrame = numFrame;
	}

	virtual wxString GetLibelle()
	{
		return libelle;
	}

	virtual void SetLibelle(const wxString& libelle)
	{
		this->libelle = libelle;
	}

	bool IsVideo()
	{
		return isVideo;
	}

	bool IsAnimation()
	{
		return isAnimation;
	}

protected:
	virtual bool TestBitmap()
	{
		return false;
	};

	cv::Mat GetDefaultPicture()
	{
		if (isVideo || isAnimation)
			return Regards::Picture::CLibPicture::mat_from_wx(defaultPictureThumbnailVideo);
		return  Regards::Picture::CLibPicture::mat_from_wx(defaultPictureThumbnailPicture);
	}

	int numCatalog;
	bool pictureLoad;
	bool pictureLoading;
	wxString filename;
	int numElement;
	int numPhotoId;
	wxString folder;
	wxString key;
	wxString libelleTimePosition;
	int numCategorie;
	int numParent;
	int typeElement;
	int timePosition;
	int percent;
	bool isProcess;
	bool forceRefresh = false;
	int numFrame = 0;
	wxString libelle;
	bool isVideo = false;
	bool isAnimation = false;
};

/*
typedef std::vector<CThumbnailData *> ThumbnailDataVector;
typedef deque<CThumbnailData *, allocator<CThumbnailData *> > DequeueThumbnailData;
typedef queue<CThumbnailData *, DequeueThumbnailData> QueueThumbnailData;
*/
