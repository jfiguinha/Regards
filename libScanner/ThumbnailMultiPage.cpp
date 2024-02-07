#include <header.h>
#include "ThumbnailMultiPage.h"
#include <Thumbnail.h>
#include <ThumbnailDataStorage.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarWnd.h"
#include <libPicture.h>
#include <picture_utility.h>
#include <LibResource.h>
using namespace Regards::Control;
using namespace Regards::Scanner;
using namespace Regards::Window;
using namespace Regards::Picture;
#define wxTIMER_PROCESS 1001

extern wxImage defaultPicture;

CThumbnailMultiPage::CThumbnailMultiPage(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
                                         const bool& testValidity)
	: CThumbnailHorizontal(parent, id, themeThumbnail, testValidity)
{
	this->parent = parent;
	numItemSelected = -1;
	process_end = true;
	moveOnPaint = false;
	threadDataProcess = true;
}

CThumbnailMultiPage::~CThumbnailMultiPage(void)
{
}

void CThumbnailMultiPage::OnPictureClick(CThumbnailData* data)
{
	int timePosition = 0;

	if (parent != nullptr && data != nullptr)
	{
		timePosition = data->GetTimePosition();
		wxCommandEvent evt(wxEVT_ANIMATIONPOSITION);
		evt.SetExtraLong(timePosition);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}

	SetVideoPosition(timePosition);
}

int CThumbnailMultiPage::FindNumItem(const int& videoPos)
{
	int numItem = 0;
	for (int i = 0; i < nbElementInIconeList; i++)
	{
		CIcone* icone = iconeList->GetElement(i);
		if (icone != nullptr)
		{
			CThumbnailData* data = icone->GetData();
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

	if (numItem >= nbElementInIconeList)
		numItem = nbElementInIconeList - 1;

	return numItem;
}

void CThumbnailMultiPage::SetVideoPosition(const int64_t& videoPos)
{
	int nbIconeElement = nbElementInIconeList;
	//wxClientDC dc(this);

	if (nbIconeElement == 0)
		return;

	int numItem = FindNumItem(videoPos);
	if (numItem == numItemSelected)
		return;

	if (numSelectPhotoId != -1)
	{
		CIcone* numSelect = GetIconeById(numSelectPhotoId);
		if (numSelect != nullptr)
			numSelect->SetSelected(false);
	}

	CIcone* pIcone = iconeList->GetElement(numItem);
	if (pIcone != nullptr)
	{
		pIcone->SetSelected(true);
	}

	if (!isMoving && pIcone != nullptr)
	{
		wxRect rect = pIcone->GetPos();
		rect.x = posLargeur + rect.x;
		rect.y = posHauteur + rect.y;

		wxWindow* parent = this->GetParent();

		if (parent != nullptr)
		{
			auto size = new wxSize();
			wxCommandEvent evt(wxEVENT_SETPOSITION);

			posHauteur = max((rect.y - this->GetWindowHeight() / 2), 0);
			posLargeur = max((rect.x - this->GetWindowWidth() / 2), 0);

			size->x = posLargeur;
			size->y = posHauteur;

			//size->x = rect.x;
			//size->y = posHauteur;
			evt.SetClientData(size);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}
		//posLargeur = rect.x;
	}

	numSelectPhotoId = iconeList->GetPhotoId(numItem);
	//numSelect = pIcone;
	//numSelect->RenderIcone(&dc);
	numItemSelected = numItem;
	needToRefresh = true;
}

void CThumbnailMultiPage::InitWithDefaultPicture(const wxString& filename,
                                                 vector<CImageVideoThumbnail*>& videoThumbnail)
{
	int x = 0;
	int y = 0;
	int typeElement = TYPEMULTIPAGE;
	threadDataProcess = false;
	auto iconeListLocal = new CIconeList();

	if (videoThumbnail.size() > 0)
	{
		int size = videoThumbnail.size();
		for (int i = 0; i < videoThumbnail.size(); i++)
		{
			CImageVideoThumbnail* thumbnail = videoThumbnail.at(i);

			float percent = (static_cast<float>(i) / static_cast<float>(size)) * 100.0f;
			auto thumbnailData = new CThumbnailDataStorage(filename);
			//thumbnailData->SetStorage(nullptr);
			thumbnailData->SetNumPhotoId(i);
			thumbnailData->SetNumElement(i);
			thumbnailData->SetTypeElement(typeElement);
			thumbnailData->SetPercent(percent);
			thumbnailData->SetLibelle("Page : " + to_string(i + 1) + "/" + to_string(size));
			thumbnailData->SetTimePosition(thumbnail->timePosition);

			if (!thumbnail->image.IsOk())
			{
				thumbnail->image = defaultPicture;
			}

			thumbnailData->SetBitmap(thumbnail->image);

			auto pBitmapIcone = new CIcone();
			pBitmapIcone->SetNumElement(i);
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);

			if (i == 0)
			{
				pBitmapIcone->SetSelected(true);
				numSelectPhotoId = i;
			}

			iconeListLocal->AddElement(pBitmapIcone);

			x += themeThumbnail.themeIcone.GetWidth();

			if (thumbnail != nullptr)
				delete thumbnail;

			thumbnail = nullptr;
		}
		processIdle = false;
	}

	iconeList = iconeListLocal;

	nbElementInIconeList = iconeList->GetNbElement();

	UpdateScroll();

	threadDataProcess = true;

	needToRefresh = true;
}

void CThumbnailMultiPage::SetFile(const wxString& filename, vector<CImageVideoThumbnail*>& videoThumbnail)
{
	InitScrollingPos();
	InitWithDefaultPicture(filename, videoThumbnail);
	needToRefresh = true;
}
