#include <header.h>
#include "ThumbnailVerticalListFile.h"
#include <ThumbnailDataSQL.h>
#include <Photos.h>
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


void CThumbnailVerticalListFile::SetListeFile(const vector<wxString>& files)
{
	std::vector<CIcone*> pIconeListToClean;
	auto iconeListLocal = new CIconeList();
	InitScrollingPos();
	threadDataProcess = false;
	int i = 0;
	int x = 0;
	int y = 0;
	thumbnailPos = 0;


	for (wxString fileEntry : files)
	{
		std::vector<CIcone*>::iterator it = std::find_if(pIconeList.begin(), pIconeList.end(), [&](CIcone* e)
			{
				CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)e->GetData();
				return thumbnailData->GetNumPhotoId() == i;

			});

		if (it == pIconeList.end())
		{
			wxString filename = fileEntry;
			auto thumbnailData = new CThumbnailDataSQL(filename, testValidity, true);
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
			pIconeList.push_back(pBitmapIcone);
		}
		else
		{
			CIcone* icone = (CIcone*)*it;
			CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)icone->GetData();
			thumbnailData->SetNumElement(i);
			icone->SetNumElement(thumbnailData->GetNumElement());
			icone->SetWindowPos(i * themeThumbnail.themeIcone.GetWidth(), 0);
			icone->SetWindowPos(x, y);
			iconeListLocal->AddElement(icone);

			if (i == 0)
				icone->SetSelected(true);
		}

		x += themeThumbnail.themeIcone.GetWidth();
		i++;
	}

	CIconeList* oldIconeList = iconeList;
	iconeList = iconeListLocal;

	nbElementInIconeList = iconeList->GetNbElement();

	for (CIcone* ico : pIconeList)
	{
		bool find = false;
		CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)ico->GetData();
		int numPhotoId = thumbnailData->GetNumPhotoId();

		for (int i = 0; i < iconeList->GetNbElement(); i++)
		{
			CIcone* ico = iconeList->GetElement(i);
			CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)ico->GetData();
			if (thumbnailData->GetNumPhotoId() == numPhotoId)
			{
				find = true;
				break;
			}
		}

		if (!find)
			pIconeListToClean.push_back(ico);
	}

	//------------------------------------
	for (CIcone* ico : pIconeListToClean)
	{
		CThumbnailDataSQL* _clean = (CThumbnailDataSQL*)ico->GetData();

		std::vector<CIcone*>::iterator it = std::find_if(pIconeList.begin(), pIconeList.end(), [&](CIcone* e)
			{
				CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)e->GetData();
				return thumbnailData->GetNumPhotoId() == _clean->GetNumPhotoId();

			});

		if (it != pIconeList.end())
			pIconeList.erase(it);
	}

	EraseThumbnailList(oldIconeList);
	EraseIconeList(pIconeListToClean);
	//----------------------------------------------------------
	// 
	//EraseThumbnailList(oldIconeList);

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
	std::vector<CIcone*> pIconeListToClean;
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
		std::vector<CIcone*>::iterator it = std::find_if(pIconeList.begin(), pIconeList.end(), [&](CIcone* e)
			{
				CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)e->GetData();
				return thumbnailData->GetNumPhotoId() == i;

			});

		if (it == pIconeList.end())
		{
			wxString filename = fileEntry;
			auto thumbnailData = new CThumbnailDataSQL(filename, testValidity, true);
			thumbnailData->SetNumPhotoId(i);
			thumbnailData->SetNumElement(i);


			auto pBitmapIcone = new CIcone();
			pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);
			iconeListLocal->AddElement(pBitmapIcone);
			pIconeList.push_back(pBitmapIcone);
		}
		else
		{
			CIcone* icone = (CIcone*)*it;
			CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)icone->GetData();
			thumbnailData->SetNumElement(i);
			icone->SetNumElement(thumbnailData->GetNumElement());
			icone->SetWindowPos(i * themeThumbnail.themeIcone.GetWidth(), 0);
			icone->SetWindowPos(x, y);
			iconeListLocal->AddElement(icone);
		}

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
		/*
		auto thumbnailData = new CThumbnailDataSQL(fileEntry, testValidity, true);
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
		*/
	}

	CIconeList* oldIconeList = iconeList;
	iconeList = iconeListLocal;

	nbElementInIconeList = iconeList->GetNbElement();

	for (CIcone* ico : pIconeList)
	{
		bool find = false;
		CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)ico->GetData();
		int numPhotoId = thumbnailData->GetNumPhotoId();

		for (int i = 0; i < iconeList->GetNbElement(); i++)
		{
			CIcone* ico = iconeList->GetElement(i);
			CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)ico->GetData();
			if (thumbnailData->GetNumPhotoId() == numPhotoId)
			{
				find = true;
				break;
			}
		}

		if (!find)
			pIconeListToClean.push_back(ico);
	}

	//------------------------------------
	for (CIcone* ico : pIconeListToClean)
	{
		CThumbnailDataSQL* _clean = (CThumbnailDataSQL*)ico->GetData();

		std::vector<CIcone*>::iterator it = std::find_if(pIconeList.begin(), pIconeList.end(), [&](CIcone* e)
			{
				CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)e->GetData();
				return thumbnailData->GetNumPhotoId() == _clean->GetNumPhotoId();

			});

		if (it != pIconeList.end())
			pIconeList.erase(it);
	}

	EraseThumbnailList(oldIconeList);
	EraseIconeList(pIconeListToClean);
	//----------------------------------------------------------
	// 
	//EraseThumbnailList(oldIconeList);

	thumbnailPos = 0;
	threadDataProcess = true;
	needToRefresh = true;
}


void CThumbnailVerticalListFile::SetListeFile(const PhotosVector& photoVector)
{
	std::vector<CIcone*> pIconeListToClean;
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
		std::vector<CIcone*>::iterator it = std::find_if(pIconeList.begin(), pIconeList.end(), [&](CIcone* e)
			{
				CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)e->GetData();
				return thumbnailData->GetNumPhotoId() == photo.GetId();

			});

		if (it == pIconeList.end())
		{
			auto thumbnailData = new CThumbnailDataSQL(photo.GetPath(), testValidity, false);
			thumbnailData->SetNumPhotoId(photo.GetId());
			thumbnailData->SetNumElement(i);

			auto pBitmapIcone = new CIcone();
			pBitmapIcone->SetNumElement(i);
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);

			iconeListLocal->AddElement(pBitmapIcone);
			pIconeList.push_back(pBitmapIcone);
		}
		else
		{
			CIcone* icone = (CIcone*)*it;
			CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)icone->GetData();
			thumbnailData->SetNumElement(i);
			icone->SetNumElement(thumbnailData->GetNumElement());
			icone->SetWindowPos(i * themeThumbnail.themeIcone.GetWidth(), 0);
			icone->SetWindowPos(x, y);
			iconeListLocal->AddElement(icone);
		}

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

	CIconeList* oldIconeList = iconeList;
	iconeList = iconeListLocal;

	nbElementInIconeList = iconeList->GetNbElement();

	for (CIcone* ico : pIconeList)
	{
		bool find = false;
		CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)ico->GetData();
		int numPhotoId = thumbnailData->GetNumPhotoId();

		for (int i = 0; i < iconeList->GetNbElement(); i++)
		{
			CIcone* ico = iconeList->GetElement(i);
			CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)ico->GetData();
			if (thumbnailData->GetNumPhotoId() == numPhotoId)
			{
				find = true;
				break;
			}
		}

		if (!find)
			pIconeListToClean.push_back(ico);
	}

	//------------------------------------
	for (CIcone* ico : pIconeListToClean)
	{
		CThumbnailDataSQL* _clean = (CThumbnailDataSQL*)ico->GetData();

		std::vector<CIcone*>::iterator it = std::find_if(pIconeList.begin(), pIconeList.end(), [&](CIcone* e)
			{
				CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)e->GetData();
				return thumbnailData->GetNumPhotoId() == _clean->GetNumPhotoId();

			});

		if (it != pIconeList.end())
			pIconeList.erase(it);
	}

	EraseThumbnailList(oldIconeList);
	EraseIconeList(pIconeListToClean);
	//----------------------------------------------------------
	// 
	//EraseThumbnailList(oldIconeList);

	threadDataProcess = true;

	AfterSetList();

	needToRefresh = true;
}
