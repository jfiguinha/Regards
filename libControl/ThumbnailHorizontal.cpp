#include <header.h>
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


CThumbnailHorizontal::CThumbnailHorizontal(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnail(parent, id, themeThumbnail, testValidity)
{
}

CThumbnailHorizontal::~CThumbnailHorizontal(void)
{
}

wxString CThumbnailHorizontal::GetWaitingMessage()
{
	return "Window CThumbnailHorizontal waiting : " + to_string(this->GetId()) + " - NbProcess Waiting : " + to_string(nbProcess);
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
		CThumbnailDataSQL * thumbnailData = new CThumbnailDataSQL(filename, testValidity);
        thumbnailData->SetNumPhotoId(i);
        thumbnailData->SetNumElement(i);


		CIcone * pBitmapIcone = new CIcone(nullptr);
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

	threadDataProcess = true;
    bufferUpdate = true;
	this->FastRefresh(this);
}


void CThumbnailHorizontal::RenderIcone(wxDC * deviceContext)
{
	int x = -posLargeur;
	int y = 0;

	//int nbPhoto = pIconeList.size();

    //int i = 0;
    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
            printf("RenderIcone Num Icone : %d \n ", i);

			//if visible
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


CIcone * CThumbnailHorizontal::FindElement(const int &xPos, const int &yPos)
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