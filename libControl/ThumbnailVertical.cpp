// ReSharper disable All
#include <header.h>
#include "ThumbnailVertical.h"
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarWnd.h"
using namespace Regards::Control;

CThumbnailVertical::CThumbnailVertical(wxWindow* parent, const wxWindowID id, const CThemeThumbnail& themeThumbnail,
                                       const bool& testValidity)
	: CThumbnail(parent, id, themeThumbnail, testValidity), test_validity_(testValidity),
	  theme_thumbnail_(themeThumbnail), id_(id)
{
	noVscroll = false;
}


CThumbnailVertical::~CThumbnailVertical(void)
= default;


CIcone* CThumbnailVertical::FindElementWithVScroll(const int& xPos, const int& yPos)
{
	int x = posLargeur + xPos;
	int y = posHauteur + yPos;

	int nbElementByX = thumbnailSizeX / themeThumbnail.themeIcone.GetWidth();
	int numY = y / themeThumbnail.themeIcone.GetHeight();
	int numX = x / themeThumbnail.themeIcone.GetWidth();
	int numElement = numY * nbElementByX + numX;

	if (numElement >= nbElementInIconeList)
		return nullptr;

	return iconeList->GetElement(numElement);
}

CIcone* CThumbnailVertical::FindElementWithoutVScroll(const int& xPos, const int& yPos)
{
	int x = posLargeur + xPos;
	if (x > thumbnailSizeX)
		return nullptr;

	int numElement = x / themeThumbnail.themeIcone.GetWidth();

	if (numElement >= nbElementInIconeList)
		return nullptr;

	return iconeList->GetElement(numElement);
}

CIcone* CThumbnailVertical::FindElement(const int& xPos, const int& yPos)
{
	if (noVscroll)
		return FindElementWithoutVScroll(xPos, yPos);

	return FindElementWithVScroll(xPos, yPos);
}

wxString CThumbnailVertical::GetWaitingMessage()
{
	return "Window CThumbnailVertical waiting : " + to_string(this->GetId()) + " - NbProcess Waiting : " +
		to_string(nbProcess);
}

void CThumbnailVertical::SetNoVScroll(const bool& noVscroll)
{
	this->noVscroll = noVscroll;
	needToRefresh = true;
}

void CThumbnailVertical::RenderIconeWithVScroll(wxDC* deviceContext)
{
	int x = -posLargeur;
	int y = -posHauteur;

	int nbElementX = 0;
	int nbElementY = 0;

	int nbElementByRow = (GetWindowWidth()) / themeThumbnail.themeIcone.GetWidth();
	if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth()))
		nbElementByRow++;

	for (int i = 0; i < nbElementInIconeList; i++)
	{
		CIcone* pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);
			pBitmapIcone->SetSizeIcone(themeThumbnail.themeIcone.GetRealWidth(),
			                           themeThumbnail.themeIcone.GetRealHeight());

			//if visible
			int left = x;
			int right = x + themeThumbnail.themeIcone.GetWidth();
			int top = y;
			int bottom = y + themeThumbnail.themeIcone.GetHeight();

			if ((right > 0 && left < GetWindowWidth()) && (top < GetWindowHeight() && bottom > 0))
			{
				if (numActifPhotoId != -1 && pBitmapIcone->GetNumElement() == 0)
				{
					numActifPhotoId = iconeList->GetPhotoId(i);
					pBitmapIcone->SetActive(true);
				}
				RenderBitmap(deviceContext, pBitmapIcone, 0, 0);
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
		}
	}
}

void CThumbnailVertical::RenderIconeWithoutVScroll(wxDC* deviceContext)
{
	int x = -posLargeur;
	int y = 0;

	for (int i = 0; i < nbElementInIconeList; i++)
	{
		CIcone* pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);
			//if visible
			int left = x;
			int right = x + themeThumbnail.themeIcone.GetWidth();
			int top = y;
			int bottom = y + themeThumbnail.themeIcone.GetHeight();

			if ((right > 0 && left < GetWindowWidth()) && (top < GetWindowHeight() && bottom > 0))
				RenderBitmap(deviceContext, pBitmapIcone, 0, 0);

			x += themeThumbnail.themeIcone.GetWidth();
		}
	}
}

void CThumbnailVertical::RenderIcone(wxDC* deviceContext)
{
	//ResizeThumbnail();
	if (noVscroll)
		RenderIconeWithoutVScroll(deviceContext);
	else
		RenderIconeWithVScroll(deviceContext);

	//scrollbar->Refresh();
}

void CThumbnailVertical::UpdateScrollWithVScroll()
{
	int oldthumbnailSizeX = thumbnailSizeX;
	int oldthumbnailSizeY = thumbnailSizeY;

	thumbnailSizeX = 0;
	thumbnailSizeY = 0;

	//bool update = false;
	int nbElement = nbElementInIconeList;

	if (nbElement > 0)
	{
		int nbElementByRow = (GetWindowWidth()) / themeThumbnail.themeIcone.GetWidth();
		if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth()))
			nbElementByRow++;

		int nbElementEnY = nbElementInIconeList / nbElementByRow;
		if (nbElementEnY * nbElementByRow < nbElementInIconeList)
			nbElementEnY++;

		if (nbElement < nbElementByRow)
			nbElementByRow = nbElement;

		thumbnailSizeX = nbElementByRow * themeThumbnail.themeIcone.GetWidth();
		thumbnailSizeY = nbElementEnY * themeThumbnail.themeIcone.GetHeight();
	}

	printf("CThumbnailVertical::UpdateScrollWithVScroll old %d %d \n", oldthumbnailSizeX, oldthumbnailSizeY);
	printf("CThumbnailVertical::UpdateScrollWithVScroll new %d %d \n", thumbnailSizeX, thumbnailSizeY);

	//bool refresh = false;
	if (nbElementInIconeList >= 0)
	{
		//int oldLargeur = posLargeur;
		//int oldHauteur = posHauteur;

		float xRatio = 1.0;
		float yRatio = 1.0;

		if (oldthumbnailSizeX != 0)
			xRatio = static_cast<float>(thumbnailSizeX) / static_cast<float>(oldthumbnailSizeX);

		if (oldthumbnailSizeY != 0)
			yRatio = static_cast<float>(thumbnailSizeY) / static_cast<float>(oldthumbnailSizeY);

		float posX = static_cast<float>(posLargeur) * xRatio;
		float posY = static_cast<float>(posHauteur) * yRatio;

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
			size->x = static_cast<int>(posX);
			size->y = static_cast<int>(posY);
			evt.SetClientData(size);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}

		posLargeur = posX;
		posHauteur = posY;
	}
}

void CThumbnailVertical::UpdateScrollWithoutVScroll()
{
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

		//UpdateScrollBar(update);
	}
}

void CThumbnailVertical::UpdateScroll()
{
	if (GetWindowWidth() <= 0)
		return;

	//printf("CThumbnailVertical::UpdateScroll \n");
	if (noVscroll)
	{
		UpdateScrollWithoutVScroll();
	}
	else
	{
		UpdateScrollWithVScroll();
	}
}
