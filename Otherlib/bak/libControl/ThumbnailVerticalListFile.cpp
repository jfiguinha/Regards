#include <header.h>
#include "ThumbnailVerticalListFile.h"
#include <ThumbnailDataSQL.h>
#include <Photos.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarWnd.h"
using namespace Regards::Control;

CThumbnailVerticalListFile::CThumbnailVerticalListFile(wxWindow* parent, wxWindowID id,
                                                       const CThemeThumbnail& themeThumbnail, const bool& testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity), numParent(0)
{
	noVscroll = false;
	typeAffichage = TYPEPHOTO;
	numCatalog = 0;
	numCategorie = 0;
	key = L"";
}

wxString CThumbnailVerticalListFile::GetWaitingMessage()
{
	return "Window CThumbnailVerticalListFile waiting : " + to_string(this->GetId()) + " - NbProcess Waiting : " +
		to_string(nbProcess);
}

void CThumbnailVerticalListFile::SetListeFile(const vector<wxString>& files)
{
	auto iconeListLocal = new CIconeList();
	InitScrollingPos();
	threadDataProcess = false;
	int i = 0;
	int x = 0;
	int y = 0;
	thumbnailPos = 0;


	for (wxString fileEntry : files)
	{
		wxString filename = fileEntry;
		auto thumbnailData = new CThumbnailDataSQL(filename, testValidity);
		thumbnailData->SetNumPhotoId(i);
		thumbnailData->SetNumElement(i);


		auto pBitmapIcone = new CIcone();
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetWindowPos(x, y);

		if (i == 0)
			pBitmapIcone->SetSelected(true);

		iconeListLocal->AddElement(pBitmapIcone);

		x += themeThumbnail.themeIcone.GetWidth();
		i++;
	}


	lockIconeList.lock();
	CIconeList* oldIconeList = iconeList;
	iconeList = iconeListLocal;
	lockIconeList.unlock();
	nbElementInIconeList = iconeList->GetNbElement();

	EraseThumbnailList(oldIconeList);

	AfterSetList();
	threadDataProcess = true;
	needToRefresh = true;
}

CThumbnailVerticalListFile::~CThumbnailVerticalListFile(void)
{
}

int CThumbnailVerticalListFile::GetTypeAffichage()
{
	return typeAffichage;
}

int CThumbnailVerticalListFile::GetNumCatalog()
{
	return numCatalog;
}

int CThumbnailVerticalListFile::GetNumCategorie()
{
	return numCategorie;
}

wxString CThumbnailVerticalListFile::GetKey()
{
	return key;
}

void CThumbnailVerticalListFile::SetListeFile(const wxArrayString& listFile, const bool& showSelectButton)
{
	auto iconeListLocal = new CIconeList();
	InitScrollingPos();
	CreateOrLoadStorageFile();
	threadDataProcess = false;

	int i = 0;
	int x = 0;
	int y = 0;

	int nbElementX = 0;
	int nbElementY = 0;

	int nbElementByRow = (GetWindowWidth()) / themeThumbnail.themeIcone.GetWidth();
	if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth()))
		nbElementByRow++;

	for (wxString fileEntry : listFile)
	{
		auto thumbnailData = new CThumbnailDataSQL(fileEntry, testValidity);
		//thumbnailData->SetStorage(pStorage->GetStoragePt());
		thumbnailData->SetNumElement(i);
		thumbnailData->SetNumPhotoId(i);

		auto pBitmapIcone = new CIcone();
		pBitmapIcone->SetNumElement(i);
		pBitmapIcone->ShowSelectButton(showSelectButton);
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetWindowPos(x, y);

		iconeListLocal->AddElement(pBitmapIcone);

		x += themeThumbnail.themeIcone.GetWidth();
		nbElementX++;
		if (nbElementX == nbElementByRow)
		{
			nbElementX = 0;
			x = -posLargeur;
			nbElementY++;
			y += themeThumbnail.themeIcone.GetHeight();
		}

		i++;
	}


	lockIconeList.lock();
	CIconeList* oldIconeList = iconeList;
	iconeList = iconeListLocal;
	lockIconeList.unlock();

	nbElementInIconeList = iconeList->GetNbElement();

	EraseThumbnailList(oldIconeList);

	thumbnailPos = 0;
	threadDataProcess = true;
	needToRefresh = true;
}


void CThumbnailVerticalListFile::SetListeFile(const PhotosVector& photoVector)
{
	InitScrollingPos();
	auto iconeListLocal = new CIconeList();
	threadDataProcess = false;
	int i = 0;
	int x = 0;
	int y = 0;

	int nbElementX = 0;
	int nbElementY = 0;

	thumbnailPos = 0;

	int nbElementByRow = (GetWindowWidth()) / themeThumbnail.themeIcone.GetWidth();
	if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth()))
		nbElementByRow++;

	for (CPhotos photo : photoVector)
	{
		auto thumbnailData = new CThumbnailDataSQL(photo.GetPath(), testValidity);
		thumbnailData->SetNumPhotoId(photo.GetId());
		thumbnailData->SetNumElement(i);

		auto pBitmapIcone = new CIcone();
		pBitmapIcone->SetNumElement(i);
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetWindowPos(x, y);

		iconeListLocal->AddElement(pBitmapIcone);

		x += themeThumbnail.themeIcone.GetWidth();
		nbElementX++;
		if (nbElementX == nbElementByRow)
		{
			nbElementX = 0;
			x = -posLargeur;
			nbElementY++;
			y += themeThumbnail.themeIcone.GetHeight();
		}

		i++;
	}


	lockIconeList.lock();
	CIconeList* oldIconeList = iconeList;
	iconeList = iconeListLocal;
	lockIconeList.unlock();

	nbElementInIconeList = iconeList->GetNbElement();

	EraseThumbnailList(oldIconeList);

	threadDataProcess = true;

	AfterSetList();

	needToRefresh = true;
}
