#include "ThumbnailHorizontal.h"
#include <ThumbnailDataSQL.h>
//#include <omp.h>
#include <queue>
#include <deque>
#include <FileUtility.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include "ScrollbarWnd.h"
using namespace Regards::Control;

CThumbnailHorizontal::CThumbnailHorizontal(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail)
	: CThumbnail(parent, id, statusBarInterface, themeThumbnail)
{
}

CThumbnailHorizontal::~CThumbnailHorizontal(void)
{
}

void CThumbnailHorizontal::InitPosition()
{
    if(scrollbar != nullptr)
    {
        scrollbar->SetPosition(0, 0);
        posHauteur = scrollbar->GetPosHauteur();
        posLargeur = scrollbar->GetPosLargeur();
    }
}

void CThumbnailHorizontal::SetListeFile(const vector<wxString> & files)
{
    threadDataProcess = false;

    InitScrollingPos();
    EraseThumbnailList();

    numSelect = nullptr;
    numActif = nullptr;
    int i = 0;
    int x = 0;
    int y = 0;
    thumbnailPos = 0;

    for (wxString fileEntry : files)
    {
        wxString filename = fileEntry;
        CThumbnailDataSQL * thumbnailData = new CThumbnailDataSQL(filename.ToStdString());
        thumbnailData->SetNumPhotoId(i);
        thumbnailData->SetNumElement(i);


        CIcone * pBitmapIcone = new CIcone();
        pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
        pBitmapIcone->SetData(thumbnailData);
        pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
        pBitmapIcone->SetWindowPos(x, y);

        if (i == 0)
            pBitmapIcone->SetSelected(true);

        pIconeList.push_back(pBitmapIcone);

        x += themeThumbnail.themeIcone.GetWidth();
        i++;

    }

	threadDataProcess = true;
   
    /*
    if(scrollbar != nullptr)
    {
        scrollbar->SetPosition(0, 0);
        posHauteur = scrollbar->GetPosHauteur();
        posLargeur = scrollbar->GetPosLargeur();
    }
     */
	this->Refresh();
}


void CThumbnailHorizontal::RenderIcone(wxDC * deviceContext)
{
	int x = -posLargeur;
	int y = 0;

	//int nbPhoto = pIconeList.size();

	for (CIcone * pBitmapIcone : pIconeList)
	{
		if (pBitmapIcone != nullptr)
		{
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);

			//if visible
			int left = x;
			int right = x + themeThumbnail.themeIcone.GetWidth();
			int top = y;
			int bottom = y + themeThumbnail.themeIcone.GetHeight();


			if ((right > 0 && left < width) && (top < height && bottom > 0))
			{
				RenderBitmap(deviceContext, pBitmapIcone);
			}
            else
            {
                pBitmapIcone->DestroyCache();
            }
			x += themeThumbnail.themeIcone.GetWidth();
		}
	}
}


void CThumbnailHorizontal::UpdateScroll()
{
	//bool update = false;

	int nbElement = (int)pIconeList.size();
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


CIcone * CThumbnailHorizontal::FindElement(const int &xPos, const int &yPos)
{
	CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();
	if (scrollH != nullptr)
	{
		int x = scrollH->GetPosition() + xPos;
		if (x > thumbnailSizeX)
			return nullptr;

		int numElement = x / themeThumbnail.themeIcone.GetWidth();

		if (numElement >= pIconeList.size())
			return nullptr;

		return pIconeList.at(numElement);
	}
	return nullptr;
}