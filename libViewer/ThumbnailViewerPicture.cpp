#include <header.h>
#include "ThumbnailViewerPicture.h"
#include "MainWindow.h"
#include <ThumbnailDataSQL.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarWnd.h>
#include <SqlFindPhotos.h>
using namespace Regards::Viewer;
using namespace Regards::Sqlite;


CThumbnailViewerPicture::CThumbnailViewerPicture(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity)
{

	widthThumbnail = 0;
	heightThumbnail = 0;
	preprocess_thumbnail = true;
}


CThumbnailViewerPicture::~CThumbnailViewerPicture(void)
{

}
void CThumbnailViewerPicture::OnPictureClick(CThumbnailData * data)
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_ONPICTURECLICK);
		evt.SetExtraLong(data->GetNumPhotoId());
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}



void CThumbnailViewerPicture::Init(const int &typeAffichage)
{
	SetListeFile();
	processIdle = true;
}

void CThumbnailViewerPicture::SetListeFile()
{
	PhotosVector pictures;
	threadDataProcess = false;

	EraseThumbnailList();

	CSqlFindPhotos sqlFindPhotos;
	sqlFindPhotos.SearchPhotos(&pictures);
	int i = 0;
	int x = 0;
	int y = 0;
	thumbnailPos = 0;

	for (CPhotos fileEntry : pictures)
	{
		wxString filename = fileEntry.GetPath();
		CThumbnailDataSQL * thumbnailData = new CThumbnailDataSQL(filename, testValidity);
		thumbnailData->SetNumPhotoId(fileEntry.GetId());
		thumbnailData->SetNumElement(i);


		CIcone * pBitmapIcone = new CIcone();
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetWindowPos(x, y);

		iconeList->AddElement(pBitmapIcone);

		x += themeThumbnail.themeIcone.GetWidth();
		i++;

	}

	AfterSetList();

	threadDataProcess = true;
	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();

	processIdle = true;

	pictures.clear();
	this->Refresh();
}

CIcone * CThumbnailViewerPicture::FindIcone(const int &photoId)
{
	int numElement = iconeList->GetNbElement();
	for (int i = 0; i < numElement; i++)
	{
		CIcone * icone = iconeList->GetElement(i);
		if (icone != nullptr)
		{
			CThumbnailData * data = icone->GetData();
			if (data != nullptr)
			{
				if (data->GetNumPhotoId() == photoId)
				{
					return icone;
				}
			}

		}

	}
	return nullptr;
}



void CThumbnailViewerPicture::ResizeThumbnail()
{
	ResizeThumbnailWithoutVScroll();

	widthThumbnail = GetWindowWidth();
	heightThumbnail = GetWindowHeight();

	UpdateScroll();
}
void CThumbnailViewerPicture::ResizeThumbnailWithoutVScroll()
{

	int x = 0;
	int y = 0;

	//int nbPhoto = pIconeList.size();

	int numElement = iconeList->GetNbElement();
	for (int i = 0; i < numElement; i++)
	{
		CIcone * pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);
			x += themeThumbnail.themeIcone.GetWidth();
		}
	}


}


void CThumbnailViewerPicture::RenderIconeWithoutVScroll(wxDC * deviceContext)
{
	//
	int numElement = iconeList->GetNbElement();
	for (int i = 0; i < numElement; i++)
	{
		CIcone * pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			wxRect rc = pBitmapIcone->GetPos();
			
			//if visible
			int left = rc.x - posLargeur;
			int right = rc.x + rc.width - posLargeur;

			if (right > 0 && left < GetWindowWidth())
			{
				pBitmapIcone->DestroyCache();
				RenderBitmap(deviceContext, pBitmapIcone, -posLargeur, 0);
			}

		}
	}
}

CIcone * CThumbnailViewerPicture::FindElement(const int &xPos, const int &yPos)
{
	int numElement = iconeList->GetNbElement();
	for (int i = 0; i < numElement; i++)
	{
		CIcone * icone = iconeList->GetElement(i);
		if (icone != nullptr)
		{
			wxRect rc = icone->GetPos();
			int left = rc.x - posLargeur;
			int right = rc.x + rc.width - posLargeur;
			int top = rc.y - posHauteur;
			int bottom = rc.y + rc.height - posHauteur;
			if ((left < xPos && xPos < right) && (top < yPos && yPos < bottom))
			{
				return icone;
			}
		}
	}

	return nullptr;
}

