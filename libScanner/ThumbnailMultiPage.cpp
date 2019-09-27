#include <header.h>
#include "ThumbnailMultiPage.h"
#include <Thumbnail.h>
#include <RegardsBitmap.h>
#include <ThumbnailDataStorage.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarWnd.h"
#include <FileUtility.h>
#include <libPicture.h>
#include <ImageLoadingFormat.h>
#include <SqlThumbnailVideo.h>
#include <picture_id.h>
using namespace Regards::Control;
using namespace Regards::Window;

#define wxTIMER_PROCESS 1001

CThumbnailMultiPage::CThumbnailMultiPage(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailHorizontal(parent, id, themeThumbnail, testValidity)
{

	this->parent = parent;
	numItemSelected = -1;
	process_end = true;
}

CThumbnailMultiPage::~CThumbnailMultiPage(void)
{
}

void CThumbnailMultiPage::OnPictureClick(CThumbnailData * data)
{
	//wxWindow * mainWindow = (wxWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (parent != nullptr && data != nullptr)
	{
		int timePosition = data->GetTimePosition();
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVT_ANIMATIONPOSITION);
		evt.SetExtraLong(timePosition);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}
}

int CThumbnailMultiPage::FindNumItem(const int &videoPos)
{
	int numItem = 0;
	int numElement = iconeList->GetNbElement();
	for (int i = 0; i < numElement; i++)
	{
		CIcone * icone = iconeList->GetElement(i);
		if (icone != nullptr)
		{
			CThumbnailData * data = icone->GetData();
			if (data != nullptr)
			{
				if (data->GetTimePosition() == videoPos)
				{
					break;
				}
			}
		}
		numItem++;
	}

	if (numItem >= numElement)
		numItem = numElement - 1;

	return numItem;
}

void CThumbnailMultiPage::SetVideoPosition(const int64_t &videoPos)
{
	int numItem = 0;
	CIcone * pIcone = nullptr;
	int nbIconeElement = iconeList->GetNbElement();
	//wxWindowDC dc(this);

	if (nbIconeElement == 0)
		return;

	numItem = FindNumItem(videoPos);
	if (numItem == numItemSelected)
		return;

	if (numSelect != nullptr)
	{
		numSelect->SetSelected(false);
		//numSelect->RenderIcone(&dc);
	}

	pIcone = iconeList->GetElement(numItem);
	if (pIcone != nullptr)
	{
		pIcone->SetSelected(true);
	}

	//Calcul NbElement By View
	int nbElement = (GetWindowWidth() / themeThumbnail.themeIcone.GetWidth());
	if ((nbElement * themeThumbnail.themeIcone.GetWidth()) < GetWindowWidth())
		nbElement++;

	int numFirstElement = posLargeur / themeThumbnail.themeIcone.GetWidth();
	int numLastElement = nbElement + numFirstElement;

	//nbElementToShow = pThumbnailDataList.size();

	if (numLastElement > (nbIconeElement - 1))
		numLastElement = nbIconeElement - 1;

	if (numFirstElement < 0)
		numFirstElement = 0;

	if (numFirstElement > (nbIconeElement - 1))
		numFirstElement = nbIconeElement - 1;

	CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();
	if (!scrollH->IsMoving() && pIcone != nullptr)
	{
		wxRect rect = pIcone->GetPos();
		rect.x = posLargeur + rect.x;
		rect.y = posHauteur + rect.y;
		scrollH->SetPosition(rect.x); scrollH->Refresh();
		posLargeur = scrollH->GetPosition();
	}

	numSelect = pIcone;
	//numSelect->RenderIcone(&dc);
	numItemSelected = numItem;
	Refresh();
}

void CThumbnailMultiPage::InitWithDefaultPicture(const wxString &filename, vector<CImageVideoThumbnail *> & videoThumbnail)
{
	int x = 0;
	int y = 0;
	int typeElement = TYPEMULTIPAGE;
	//int rotation = 0;
	EraseThumbnailList();

	if (videoThumbnail.size() > 0)
	{
		int size = videoThumbnail.size();
		for (int i = 0; i < videoThumbnail.size(); i++)
		{
			CImageVideoThumbnail * thumbnail = videoThumbnail.at(i);

			float percent = ((float)i / (float)size) * 100.0f;
			CThumbnailDataStorage * thumbnailData = new CThumbnailDataStorage(filename);
			//thumbnailData->SetStorage(nullptr);
			thumbnailData->SetNumPhotoId(i);
			thumbnailData->SetNumElement(i);
			thumbnailData->SetTypeElement(typeElement);
			thumbnailData->SetPercent(percent);
			thumbnailData->SetLibelle("Page : " + to_string(i + 1) + "/" + to_string(size));
			thumbnailData->SetTimePosition(thumbnail->timePosition);
			thumbnailData->SetBitmap(thumbnail->image);

			CIcone * pBitmapIcone = new CIcone(nullptr);
			pBitmapIcone->SetNumElement(i);
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);

			if (i == 0)
			{
				pBitmapIcone->SetSelected(true);
				numSelect = pBitmapIcone;
			}

			iconeList->AddElement(pBitmapIcone);

			x += themeThumbnail.themeIcone.GetWidth();

			if (thumbnail != nullptr)
				delete thumbnail;

			thumbnail = nullptr;
		}
		processIdle = false;
	}



}
void CThumbnailMultiPage::SetFile(const wxString &filename, vector<CImageVideoThumbnail *> & videoThumbnail)
{
	InitScrollingPos();
	InitWithDefaultPicture(filename, videoThumbnail);
	Refresh();
}
