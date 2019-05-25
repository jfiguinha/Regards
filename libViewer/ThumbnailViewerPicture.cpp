#include <header.h>
#include "ThumbnailViewerPicture.h"
#include "MainWindow.h"
#include <window_id.h>
#include <ThumbnailDataSQL.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarVerticalWnd.h>
#include <ScrollbarWnd.h>

using namespace Regards::Viewer;
using namespace Regards::Sqlite;


CThumbnailViewerPicture::CThumbnailViewerPicture(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity)
{

	widthThumbnail = 0;
	heightThumbnail = 0;
}


CThumbnailViewerPicture::~CThumbnailViewerPicture(void)
{

}

void CThumbnailViewerPicture::SetActifItem(const int &numItem, const bool &move)
{
	this->SetFocus();

	if (numItem >= iconeList->GetNbElement())
		return;

	CIcone * numItemSelected = iconeList->GetElement(numItem);

	if (numItemSelected == nullptr)
		return;     

	if (numSelect != NULL)
		numSelect->SetSelected(false);

	if (numActif != nullptr)
	{
		numActif->SetActive(false);
	}

	numActif = iconeList->GetElement(numItem);

	if (move)
	{
		CScrollbarVerticalWnd * scrollV = scrollbar->GetVScrollbar();
		CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();

		if (numItem == 0)
		{
			scrollV->SetPosition(0);
			scrollH->SetPosition(0);
			InitScrollingPos();
		}
		else {
			if (!scrollV->IsMoving())
			{
				wxRect rect = numActif->GetPos();

				int yPos = max((rect.y - scrollV->GetScreenHeight() / 2), 0);
				int xPos = max((rect.x - scrollH->GetScreenWidth() / 2), 0);

				scrollV->SetPosition(yPos);
				scrollH->SetPosition(xPos);


				if (GetWindowWidth() < GetWidth())
					posLargeur = scrollH->GetPosition();

				if (GetWindowHeight() < GetHeight())
					posHauteur = scrollV->GetPosition();

			}
		}
	}



	numSelect = iconeList->GetElement(numItem);

	if (numSelect != nullptr)
	{
		numSelect->SetSelected(true);
		//numSelect->RenderIcone(&winDC);
	}
	this->Refresh();
}

void CThumbnailViewerPicture::OnPictureClick(CThumbnailData * data)
{
	CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ONPICTURECLICK);
		evt.SetExtraLong(data->GetNumPhotoId());
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}



void CThumbnailViewerPicture::Init(PhotosVector * photoVector, const int &typeAffichage)
{
	if (photoVector != nullptr)
	{
		SetListeFile(photoVector);
	}
	processIdle = true;
}

void CThumbnailViewerPicture::SetListeFile(PhotosVector * photoVector)
{
	threadDataProcess = false;

	EraseThumbnailList();

	numSelect = nullptr;
	numActif = nullptr;
	int i = 0;
	int x = 0;
	int y = 0;
	thumbnailPos = 0;

	for (CPhotos fileEntry : *photoVector)
	{
		wxString filename = fileEntry.GetPath();
		CThumbnailDataSQL * thumbnailData = new CThumbnailDataSQL(filename, testValidity);
		thumbnailData->SetNumPhotoId(fileEntry.GetId());
		thumbnailData->SetNumElement(i);


		CIcone * pBitmapIcone = new CIcone(nullptr);
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetWindowPos(x, y);

		iconeList->AddElement(pBitmapIcone);

		x += themeThumbnail.themeIcone.GetWidth();
		i++;

	}

	threadDataProcess = true;
	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();

	processIdle = true;
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

	posLargeur = scrollbar->GetPosLargeur();

	//#pragma omp parallel for
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

