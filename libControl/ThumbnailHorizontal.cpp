#include <header.h>
#include "ThumbnailHorizontal.h"
#include <ThumbnailDataSQL.h>
#include <queue>
#include "ScrollbarHorizontalWnd.h"
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
	wxWindow * parent = this->GetParent();
	if (parent != nullptr)
	{
		wxSize * size = new wxSize();
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		size->x = posLargeur;
		size->y = posHauteur;
		evt.SetClientData(size);
		parent->GetEventHandler()->AddPendingEvent(evt);
	}

	posHauteur = 0;
	posLargeur = 0;


}

void CThumbnailHorizontal::SetListeFile(const vector<wxString> & files)
{
    threadDataProcess = false;
	this->SetFocus();
    InitScrollingPos();
	CIconeList* iconeListLocal = new CIconeList();
	CIconeList* oldIconeList = nullptr;

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


		CIcone * pBitmapIcone = new CIcone();
        pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
        pBitmapIcone->SetData(thumbnailData);
        pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
        pBitmapIcone->SetWindowPos(x, y);

        if (i == 0)
            pBitmapIcone->SetSelected(true);

		iconeListLocal->AddElement(pBitmapIcone);

        x += themeThumbnail.themeIcone.GetWidth();
        i++;

    }

	 lockIconeList.lock();
	 oldIconeList = iconeList;
	 iconeList = iconeListLocal;
	 lockIconeList.unlock();

	 EraseThumbnailList(oldIconeList);

	threadDataProcess = true;
	AfterSetList();
	Refresh();
}


void CThumbnailHorizontal::RenderIcone(wxDC * deviceContext)
{
	int x = -posLargeur;
	int y = 0;

	for (int i = 0;i < nbElementInIconeList;i++)
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
	if (GetWindowWidth() <= 0)
		return;

	int nbElement = (int)nbElementInIconeList;
	if (nbElement > 0)
	{
		nbLigneY = 1;
		nbLigneX = nbElement;
		thumbnailSizeX = nbLigneX * themeThumbnail.themeIcone.GetWidth();
		thumbnailSizeY = themeThumbnail.themeIcone.GetHeight();

		wxWindow * parent = this->GetParent();

		if (parent != nullptr)
		{
			CControlSize * controlSize = new CControlSize();
			wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
			controlSize->controlWidth = thumbnailSizeX;
			controlSize->controlHeight = thumbnailSizeY;
			evt.SetClientData(controlSize);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}

		if (parent != nullptr)
		{
			wxSize * size = new wxSize();
			wxCommandEvent evt(wxEVENT_SETPOSITION);
			size->x = posLargeur;
			size->y = posHauteur;
			evt.SetClientData(size);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}
	}

}


CIcone * CThumbnailHorizontal::FindElement(const int &xPos, const int &yPos)
{
	int x = posLargeur + xPos;
	if (x > thumbnailSizeX)
		return nullptr;

	int numElement = x / themeThumbnail.themeIcone.GetWidth();

	if (numElement >= nbElementInIconeList)
		return nullptr;

	return iconeList->GetElement(numElement);
}