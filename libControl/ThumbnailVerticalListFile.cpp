#include <header.h>
#include "ThumbnailVerticalListFile.h"
#include <ThumbnailDataSQL.h>
#include <Photos.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarWnd.h"
using namespace Regards::Control;

CThumbnailVerticalListFile::CThumbnailVerticalListFile(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity)
{
	noVscroll = false;
	typeAffichage = TYPEPHOTO;
	numCatalog = 0;
	numCategorie = 0;
	key = L"";
}

wxString CThumbnailVerticalListFile::GetWaitingMessage()
{
	return "Window CThumbnailVerticalListFile waiting : " + to_string(this->GetId()) + " - NbProcess Waiting : " + to_string(nbProcess);
}

void CThumbnailVerticalListFile::SetListeFile(const vector<wxString> & files)
{
	threadDataProcess = false;

	InitScrollingPos();
	EraseThumbnailList();

	int i = 0;
	int x = 0;
	int y = 0;
	thumbnailPos = 0;


	 for (wxString fileEntry : files)
	{
		wxString filename = fileEntry;
		CThumbnailDataSQL * thumbnailData = new CThumbnailDataSQL(filename, testValidity);
		thumbnailData->SetNumPhotoId(i);
		thumbnailData->SetNumElement(i);


		CIcone * pBitmapIcone = new CIcone();
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetWindowPos(x, y);

		if (i == 0)
			pBitmapIcone->SetSelected(true);

		iconeList->AddElement(pBitmapIcone);

		x += themeThumbnail.themeIcone.GetWidth();
		i++;

	}

	AfterSetList();
	threadDataProcess = true;
	Refresh();
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

void CThumbnailVerticalListFile::SetListeFile(const wxArrayString & listFile, const bool &showSelectButton)
{
    threadDataProcess = false;
    InitScrollingPos();
    EraseThumbnailList();
    CreateOrLoadStorageFile();

    int i = 0;
    int x = 0;
    int y = 0;

    int nbElementX = 0;
    int nbElementY = 0;

    int nbElementByRow = (GetWindowWidth()) / themeThumbnail.themeIcone.GetWidth();
    if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) <  (GetWindowWidth()))
        nbElementByRow++;

    int nbElementEnY = (int)listFile.size() / nbElementByRow;
    if (nbElementEnY * nbElementByRow < listFile.size())
        nbElementEnY++;


	 for (wxString fileEntry : listFile)
    {
		CThumbnailDataSQL * thumbnailData = new CThumbnailDataSQL(fileEntry, testValidity);
        //thumbnailData->SetStorage(pStorage->GetStoragePt());
        thumbnailData->SetNumElement(i);
        thumbnailData->SetNumPhotoId(i);

		CIcone * pBitmapIcone = new CIcone();
        pBitmapIcone->SetNumElement(i);
        pBitmapIcone->ShowSelectButton(showSelectButton);
        pBitmapIcone->SetData(thumbnailData);
        pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
        pBitmapIcone->SetWindowPos(x, y);

        iconeList->AddElement(pBitmapIcone);

        x += themeThumbnail.themeIcone.GetWidth(); nbElementX++;
        if (nbElementX == nbElementByRow)
        {
            nbElementX = 0;
            x = -posLargeur;
            nbElementY++;
            y += themeThumbnail.themeIcone.GetHeight();
        }
        
        i++;

    }
    SetNbFiles(i);

    thumbnailPos = 0;
    
	EraseThumbnailList();

    threadDataProcess = true;

}


void CThumbnailVerticalListFile::SetListeFile(const PhotosVector & photoVector, const bool &erase)
{

    if (erase)
    {
        InitScrollingPos();
        EraseThumbnailList();

		loadingIcone = nullptr;
		numSelect = nullptr;
		numActif = nullptr;
    }



    int i = 0;
    int x = 0;
    int y = 0;

    int nbElementX = 0;
    int nbElementY = 0;
    
    thumbnailPos = 0;

    int nbElementByRow = (GetWindowWidth()) / themeThumbnail.themeIcone.GetWidth();
    if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) <  (GetWindowWidth()))
        nbElementByRow++;

    int nbElementEnY = (int)photoVector.size() / nbElementByRow;
    if (nbElementEnY * nbElementByRow < photoVector.size())
        nbElementEnY++;

    for (CPhotos photo : photoVector)
    {
		CThumbnailDataSQL * thumbnailData = new CThumbnailDataSQL(photo.GetPath(), testValidity);
        thumbnailData->SetNumPhotoId(photo.GetId());
        thumbnailData->SetNumElement(i);

		CIcone * pBitmapIcone = new CIcone();
        pBitmapIcone->SetNumElement(i);
        pBitmapIcone->SetData(thumbnailData);
        pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
        pBitmapIcone->SetWindowPos(x, y);

        iconeList->AddElement(pBitmapIcone);

        x += themeThumbnail.themeIcone.GetWidth(); nbElementX++;
        if (nbElementX == nbElementByRow)
        {
            nbElementX = 0;
            x = -posLargeur;
            nbElementY++;
            y += themeThumbnail.themeIcone.GetHeight();
        }
        
        i++;
    }
    SetNbFiles(i);

	if (erase)
	{
		AfterSetList();
	}

	Refresh();
}


