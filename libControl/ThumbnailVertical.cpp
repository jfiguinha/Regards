#include <header.h>
#include "ThumbnailVertical.h"
#include <ThumbnailDataSQL.h>
#include <Photos.h>
#include <FileUtility.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include "ScrollbarWnd.h"
using namespace Regards::Control;

CThumbnailVertical::CThumbnailVertical(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnail(parent, id, themeThumbnail, testValidity)
{
	noVscroll = false;
}


CThumbnailVertical::~CThumbnailVertical(void)
{

}


CIcone * CThumbnailVertical::FindElementWithVScroll(const int &xPos, const int &yPos)
{
	CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();
	CScrollbarVerticalWnd * scrollV = scrollbar->GetVScrollbar();
	if (scrollH != nullptr && scrollV != nullptr)
	{

		int x = scrollH->GetPosition() + xPos;
		int y = scrollV->GetPosition() + yPos;

		int nbElementByX = thumbnailSizeX / themeThumbnail.themeIcone.GetWidth();
		int numY = y / themeThumbnail.themeIcone.GetHeight();
		int numX = x / themeThumbnail.themeIcone.GetWidth();
		int numElement = numY * nbElementByX + numX;

		if (numElement >= iconeList->GetNbElement())
			return nullptr;

		return iconeList->GetElement(numElement);
	}
	return nullptr;
}

CIcone * CThumbnailVertical::FindElementWithoutVScroll(const int &xPos, const int &yPos)
{
	CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();
	if (scrollH != nullptr)
	{
		int x = scrollH->GetPosition() + xPos;
		if (x > thumbnailSizeX)
			return nullptr;

		int numElement = x / themeThumbnail.themeIcone.GetWidth();

		if (numElement >= iconeList->GetNbElement())
			return nullptr;

		return iconeList->GetElement(numElement);
	}
	return nullptr;
}

CIcone * CThumbnailVertical::FindElement(const int &xPos, const int &yPos)
{
	if (noVscroll)
		return FindElementWithoutVScroll(xPos, yPos);

	return FindElementWithVScroll(xPos, yPos);

}

void CThumbnailVertical::SetNoVScroll(const bool &noVscroll)
{
	this->noVscroll = noVscroll;
	Refresh();
}

void CThumbnailVertical::RenderIconeWithVScroll(wxDC * deviceContext)
{
	int x = -posLargeur;
	int y = -posHauteur;

	int nbElementX = 0;
	int nbElementY = 0;

	int nbElementByRow = (GetWindowWidth() + positionXOld) / themeThumbnail.themeIcone.GetWidth();
	if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) <  (GetWindowWidth() + positionXOld))
		nbElementByRow++;

	int nbElementEnY = (int)iconeList->GetNbElement() / nbElementByRow;
	if (nbElementEnY * nbElementByRow < iconeList->GetNbElement())
		nbElementEnY++;



    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * pBitmapIcone = iconeList->GetElement(i);
        if(pBitmapIcone != nullptr)
        {
      		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
            pBitmapIcone->SetWindowPos(x, y);
            pBitmapIcone->SetSizeIcone(themeThumbnail.themeIcone.GetRealWidth(), themeThumbnail.themeIcone.GetRealHeight());


            //if visible
            int left = x;
            int right = x + themeThumbnail.themeIcone.GetWidth();
            int top = y;
            int bottom = y + themeThumbnail.themeIcone.GetHeight();

            if ((right > 0 && left < GetWindowWidth()) && (top < GetWindowHeight() && bottom > 0))
            {
                if (numActif == nullptr && pBitmapIcone->GetNumElement() == 0)
                {
                    numActif = pBitmapIcone;
                    pBitmapIcone->SetActive(true);
                }
                RenderBitmap(deviceContext, pBitmapIcone, 0, 0);
            }
            else
            {
                pBitmapIcone->DestroyCache();
            }


            x += themeThumbnail.themeIcone.GetWidth(); nbElementX++;
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

void CThumbnailVertical::RenderIconeWithoutVScroll(wxDC * deviceContext)
{
	posLargeur = scrollbar->GetPosLargeur();
	int x = -posLargeur;
	int y = 0;

	//int nbPhoto = pIconeList.size();

    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * pBitmapIcone = iconeList->GetElement(i);
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
			{
				RenderBitmap(deviceContext, pBitmapIcone, 0, 0);
			}
			else
			{
				pBitmapIcone->DestroyCache();
			}
			x += themeThumbnail.themeIcone.GetWidth();
		}
	}
}

void CThumbnailVertical::RenderIcone(wxDC * deviceContext)
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
	int nbElement = (int)iconeList->GetNbElement();

	if (nbElement > 0)
	{
		int nbElementByRow = (GetWindowWidth() + positionXOld) / themeThumbnail.themeIcone.GetWidth();
		if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth() + positionXOld))
			nbElementByRow++;

		int nbElementEnY = (int)iconeList->GetNbElement() / nbElementByRow;
		if (nbElementEnY * nbElementByRow < iconeList->GetNbElement())
			nbElementEnY++;

		if (nbElement < nbElementByRow)
			nbElementByRow = nbElement;

		thumbnailSizeX = nbElementByRow * themeThumbnail.themeIcone.GetWidth();
		thumbnailSizeY = nbElementEnY * themeThumbnail.themeIcone.GetHeight();
	}
    
    printf("CThumbnailVertical::UpdateScrollWithVScroll old %d %d \n", oldthumbnailSizeX, oldthumbnailSizeY);
    printf("CThumbnailVertical::UpdateScrollWithVScroll new %d %d \n", thumbnailSizeX, thumbnailSizeY);

	//bool refresh = false;
	if (iconeList->GetNbElement() >= 0)
	{
		//int oldLargeur = posLargeur;
		//int oldHauteur = posHauteur;

		float xRatio = 1.0;
		float yRatio = 1.0;

		if (oldthumbnailSizeX != 0)
			xRatio = (float)thumbnailSizeX / (float)oldthumbnailSizeX;

		if (oldthumbnailSizeY != 0)
			yRatio = (float)thumbnailSizeY / (float)oldthumbnailSizeY;

		float posX = (float)posLargeur * xRatio;
		float posY = (float)posHauteur * yRatio;

		scrollbar->SetControlSize(thumbnailSizeX, thumbnailSizeY);
		scrollbar->SetPosition(posX, posY);

		posLargeur = scrollbar->GetPosLargeur();
		posHauteur = scrollbar->GetPosHauteur();
	}
}

void CThumbnailVertical::UpdateScrollWithoutVScroll()
{
	int nbElement = (int)iconeList->GetNbElement();
	if (nbElement > 0)
	{
		nbLigneY = 1;
		nbLigneX = nbElement;
		thumbnailSizeX = nbLigneX * themeThumbnail.themeIcone.GetWidth();
		thumbnailSizeY = themeThumbnail.themeIcone.GetHeight();

		scrollbar->SetControlSize(thumbnailSizeX, thumbnailSizeY);
		scrollbar->SetPosition(posLargeur, posHauteur);
		//UpdateScrollBar(update);
	}

	posHauteur = scrollbar->GetPosHauteur();
	posLargeur = scrollbar->GetPosLargeur();
}

void CThumbnailVertical::UpdateScroll()
{
    printf("CThumbnailVertical::UpdateScroll \n");
	if (noVscroll)
	{
		UpdateScrollWithoutVScroll();
	}
	else
	{
		UpdateScrollWithVScroll();
	}

}

