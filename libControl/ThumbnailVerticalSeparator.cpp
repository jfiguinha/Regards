// ReSharper disable All
#include <header.h>
#include "ThumbnailVerticalSeparator.h"
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarWnd.h"
using namespace Regards::Control;

CThumbnailVerticalSeparator::CThumbnailVerticalSeparator(wxWindow* parent, const wxWindowID id, const CThemeThumbnail& themeThumbnail,
                                       const bool& testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity)
{

}


CThumbnailVerticalSeparator::~CThumbnailVerticalSeparator(void)
= default;



void CThumbnailVerticalSeparator::ResizeThumbnail()
{
	int x = 0;
	int y = 0;

	int nbElementX = 0;
	int nbElementY = 0;

	int nbElementByRow = 0;

	//Calcul du width max
	for (auto i = 0; i < listSeparator.size(); i++)
	{
		//int nbElement = infosSeparationBar->listElement.size();

		int elementByRow = (GetWindowWidth()) / themeThumbnail.themeIcone.GetWidth();
		if ((elementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth()))
			elementByRow++;

		if (nbElementByRow < elementByRow)
			nbElementByRow = elementByRow;
	}

	int controlWidth = nbElementByRow * themeThumbnail.themeIcone.GetWidth();

	for (CInfosSeparationBar* infosSeparationBar : listSeparator)
	{
		int nbElementEnY = 0;
		int nbElement = static_cast<int>(infosSeparationBar->listElement.size());
		if (nbElementByRow > 0)
		{
			nbElementEnY = nbElement / nbElementByRow;
			if (nbElementEnY * nbElementByRow < nbElement)
				nbElementEnY++;
		}

		infosSeparationBar->SetWidth(controlWidth);
		infosSeparationBar->SetWindowPos(x, y);

		y += infosSeparationBar->GetHeight();

		for (auto numElement : infosSeparationBar->listElement)
		{
			CIcone* pBitmapIcone = iconeList->GetElement(numElement);
			if (pBitmapIcone != nullptr)
			{
				pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
				pBitmapIcone->SetWindowPos(x, y);

				x += themeThumbnail.themeIcone.GetWidth();
				nbElementX++;
				if (nbElementX == nbElementByRow)
				{
					nbElementX = 0;
					x = 0;
					nbElementY++;
					y += themeThumbnail.themeIcone.GetHeight();
				}
			}
		}

		if (nbElementX != 0)
		{
			nbElementX = 0;
			x = 0;
			nbElementY++;
			y += themeThumbnail.themeIcone.GetHeight();
		}
	}

	widthThumbnail = GetWindowWidth();
	heightThumbnail = GetWindowHeight();

	UpdateScroll();
}

void CThumbnailVerticalSeparator::ResizeThumbnailWithVScroll()
{
	int x = 0;
	int y = 0;

	int nbElementX = 0;
	int nbElementY = 0;

	int nbElementByRow = 0;

	//Calcul du width max
	for (auto i = 0; i < listSeparator.size(); i++)
	{
		//int nbElement = infosSeparationBar->listElement.size();

		int elementByRow = (GetWindowWidth()) / themeThumbnail.themeIcone.GetWidth();
		if ((elementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth()))
			elementByRow++;

		if (nbElementByRow < elementByRow)
			nbElementByRow = elementByRow;
	}

	int controlWidth = nbElementByRow * themeThumbnail.themeIcone.GetWidth();

	for (CInfosSeparationBar* infosSeparationBar : listSeparator)
	{
		int nbElementEnY = 0;
		int nbElement = static_cast<int>(infosSeparationBar->listElement.size());
		if (nbElementByRow > 0)
		{
			nbElementEnY = nbElement / nbElementByRow;
			if (nbElementEnY * nbElementByRow < nbElement)
				nbElementEnY++;
		}

		infosSeparationBar->SetWidth(controlWidth);
		infosSeparationBar->SetWindowPos(x, y);

		y += infosSeparationBar->GetHeight();

		for (auto numElement : infosSeparationBar->listElement)
		{
			CIcone* pBitmapIcone = iconeList->GetElement(numElement);
			if (pBitmapIcone != nullptr)
			{
				pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
				pBitmapIcone->SetWindowPos(x, y);

				x += themeThumbnail.themeIcone.GetWidth();
				nbElementX++;
				if (nbElementX == nbElementByRow)
				{
					nbElementX = 0;
					x = 0;
					nbElementY++;
					y += themeThumbnail.themeIcone.GetHeight();
				}
			}
		}

		if (nbElementX != 0)
		{
			nbElementX = 0;
			x = 0;
			nbElementY++;
			y += themeThumbnail.themeIcone.GetHeight();
		}
	}
}


void CThumbnailVerticalSeparator::RenderIconeWithVScroll(wxDC* deviceContext)
{
	for (auto i = 0; i < listSeparator.size(); i++)
	{
		CInfosSeparationBar* infosSeparationBar = listSeparator.at(i);
		infosSeparationBar->Render(deviceContext, -posLargeur, -posHauteur);
		bool isStart = false;

		for (auto j = 0; j < infosSeparationBar->listElement.size(); j++)
		{
			int numElement = infosSeparationBar->listElement.at(j);
			CIcone* pBitmapIcone = iconeList->GetElement(numElement);
			if (pBitmapIcone != nullptr)
			{
				wxRect rc = pBitmapIcone->GetPos();
				int left = rc.x - posLargeur;
				int right = rc.x + rc.width - posLargeur;
				int top = rc.y - posHauteur;
				int bottom = rc.y + rc.height - posHauteur;

				if ((right > 0 && left < GetWindowWidth()) && (top < GetWindowHeight() && bottom > 0))
				{
					isStart = true;
					RenderBitmap(deviceContext, pBitmapIcone, -posLargeur, -posHauteur);
				}
				else if (isStart)
					break;
			}
		}
	}
}

void CThumbnailVerticalSeparator::UpdateScrollWithVScroll()
{
	//printf("CThumbnailFace::UpdateScrollWithVScroll \n");
	//bool update = false;
	int oldthumbnailSizeX = thumbnailSizeX;
	int oldthumbnailSizeY = thumbnailSizeY;

	thumbnailSizeX = 0;
	thumbnailSizeY = 0;

	for (CInfosSeparationBar* infosSeparationBar : listSeparator)
	{
		int nbElement = static_cast<int>(infosSeparationBar->listElement.size());

		int nbElementByRow = (GetWindowWidth()) / themeThumbnail.themeIcone.GetWidth();

		if (nbElement > 0 && nbElementByRow == 0)
		{
			float value = static_cast<float>(GetWindowWidth()) / static_cast<float>(themeThumbnail.themeIcone.
				GetWidth());
			if (value > 0)
				nbElementByRow = 1;
		}

		if (nbElementByRow > 0)
		{
			if (nbElementByRow < 1)
				nbElementByRow = 1;

			if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth()))
				nbElementByRow++;

			int nbElementEnY = static_cast<int>(infosSeparationBar->listElement.size()) / nbElementByRow;
			if (nbElementEnY * nbElementByRow < infosSeparationBar->listElement.size())
				nbElementEnY++;

			if (nbElement < nbElementByRow)
				nbElementByRow = nbElement;

			int sizeX = nbElementByRow * themeThumbnail.themeIcone.GetWidth();
			if (sizeX > thumbnailSizeX)
				thumbnailSizeX = nbElementByRow * themeThumbnail.themeIcone.GetWidth();
			thumbnailSizeY += nbElementEnY * themeThumbnail.themeIcone.GetHeight() + infosSeparationBar->GetHeight();
		}
		else
			break;
	}


	//printf("CThumbnailFace::UpdateScrollWithVScroll old %d %d \n", oldthumbnailSizeX, oldthumbnailSizeY);
	//printf("CThumbnailFace::UpdateScrollWithVScroll new %d %d \n", thumbnailSizeX, thumbnailSizeY);


	//bool refresh = false;
	if (nbElementInIconeList >= 0)
	{
		//int oldLargeur = posLargeur;
		//int oldHauteur = posHauteur;

		if (posLargeur < 0)
			posLargeur = 0;

		if (posHauteur < 0)
			posHauteur = 0;

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
			size->x = posX;
			size->y = posY;
			evt.SetClientData(size);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}

		posLargeur = posX;
		posHauteur = posY;
	}
	needToRefresh = true;
}