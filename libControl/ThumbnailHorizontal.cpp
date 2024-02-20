#include <header.h>
#include "ThumbnailHorizontal.h"
#include <ThumbnailDataSQL.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarWnd.h"
using namespace Regards::Control;


CThumbnailHorizontal::CThumbnailHorizontal(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
                                           const bool& testValidity)
	: CThumbnail(parent, id, themeThumbnail, testValidity)
{
}

CThumbnailHorizontal::~CThumbnailHorizontal(void)
{
}

void CThumbnailHorizontal::InitPosition()
{
	wxWindow* parent = this->GetParent();
	if (parent != nullptr)
	{
		auto size = new wxSize();
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		size->x = posLargeur;
		size->y = posHauteur;
		evt.SetClientData(size);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}

	posHauteur = 0;
	posLargeur = 0;
}

void CThumbnailHorizontal::SetListeFile(const vector<wxString>& files)
{
	std::vector<CIcone*> pIconeListToClean;
	this->SetFocus();
	InitScrollingPos();
	auto iconeListLocal = new CIconeList();
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

	auto oldIconeList = iconeList;
	iconeList = iconeListLocal;

	nbElementInIconeList = iconeList->GetNbElement();

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


void CThumbnailHorizontal::RenderIcone(wxDC* deviceContext)
{
	int x = -posLargeur;
	int y = 0;

	for (int i = 0; i < nbElementInIconeList; i++)
	{
		CIcone * pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
			int left = x;
			int right = x + themeThumbnail.themeIcone.GetWidth();
			int top = y;
			int bottom = y + themeThumbnail.themeIcone.GetHeight();
			pBitmapIcone->SetWindowPos(x, y);
			if ((right > 0 && left < GetWindowWidth()) && (top < GetWindowHeight() && bottom > 0))
			{
				pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
				RenderBitmap(deviceContext, pBitmapIcone, 0, 0);
			}

			x += themeThumbnail.themeIcone.GetWidth();
		}
	}
}


void CThumbnailHorizontal::UpdateScroll()
{
	//bool update = false;
	if (GetWindowWidth() <= 0)
		return;

	int nbElement = nbElementInIconeList;
	if (nbElement > 0)
	{
		nbLigneY = 1;
		nbLigneX = nbElement;
		thumbnailSizeX = nbLigneX * themeThumbnail.themeIcone.GetWidth();
		thumbnailSizeY = themeThumbnail.themeIcone.GetHeight();

		wxWindow* parent = this->GetParent();

		if (parent != nullptr)
		{
			auto controlSize = new CControlSize();
			wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
			controlSize->controlWidth = thumbnailSizeX;
			controlSize->controlHeight = thumbnailSizeY;
			evt.SetClientData(controlSize);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}

		if (parent != nullptr)
		{
			auto size = new wxSize();
			wxCommandEvent evt(wxEVENT_SETPOSITION);
			size->x = posLargeur;
			size->y = posHauteur;
			evt.SetClientData(size);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}
	}
}


CIcone * CThumbnailHorizontal::FindElement(const int& xPos, const int& yPos)
{
	int x = posLargeur + xPos;
	if (x > thumbnailSizeX)
		return nullptr;

	int numElement = x / themeThumbnail.themeIcone.GetWidth();

	if (numElement >= nbElementInIconeList)
		return nullptr;

	return iconeList->GetElement(numElement);
}
