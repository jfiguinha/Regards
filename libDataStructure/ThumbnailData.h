#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "RegardsBitmap.h"
#include <queue>
#include <deque>
#include <vector>
using namespace std;

#define TYPEPHOTO 1
#define TYPECATALOG 2
#define TYPECATEGORIE 3
#define TYPECRITERIA 4
#define TYPEFOLDER 5
#define TYPEVIDEO 6
#define SQLITE 1


class CThumbnailData
{
public:
	CThumbnailData(const wxString & filename);
	virtual ~CThumbnailData(void);

	virtual int GetType() = 0;

	void SetNumPhotoId(const int &id);
	int GetNumPhotoId();

	void SetNumElement(const int &iNumElement);
	int GetNumElement();

	wxString GetFilename();

	virtual void SetBitmap(CRegardsBitmap * bitmap) = 0;
	virtual CRegardsBitmap * GetRegardsBitmap(const int &type) = 0;
	virtual wxImage GetwxImage() = 0;

	void SetTypeElement(const int &typeElement);
	int GetTypeElement();

	void SetNumCatalog(const int &numCatalog);
	int GetNumCatalog();

	void SetNumCategorie(const int &numCategorie);
	int GetNumCategorie();

	void SetNumParent(const int &numParent);
	int GetNumParent();

	void SetKey(const wxString &key);
	wxString GetKey();

	bool IsLoading();
	void SetIsLoading(const bool &isLoading);

	bool IsLoad();
	void InitLoadState()
	{
		pictureLoad = false;
		pictureLoading = false;
	}

	int GetPercent();
	void SetPercent(const int &percent);

	void SetTimePosition(const int &timePosition);
	int GetTimePosition();
	wxString GetTimePositionLibelle();

protected:

	virtual bool TestBitmap() = 0;
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
	int timePosition = 0;
	int percent = 10;

};

typedef std::vector<CThumbnailData *> ThumbnailDataVector;
typedef deque<CThumbnailData *, allocator<CThumbnailData *> > DequeueThumbnailData;
typedef queue<CThumbnailData *, DequeueThumbnailData> QueueThumbnailData;