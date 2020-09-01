#include <header.h>
#include "ThumbnailFileSelection.h"
#include <window_id.h>
#include <LibResource.h>
#include <ThumbnailDataStorage.h>
#include <libPicture.h>
#include <ConvertUtility.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarVerticalWnd.h>
#include <ScrollbarWnd.h>
#include <ImageVideoThumbnail.h>
using namespace Regards::Scanner;
using namespace Regards::Sqlite;

CThumbnailFileSelection::CThumbnailFileSelection(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity)
{
	
	barseparationHeight = 40;
	widthThumbnail = 0;
	heightThumbnail = 0;
}


CThumbnailFileSelection::~CThumbnailFileSelection(void)
{

}

void CThumbnailFileSelection::OnPictureClick(CThumbnailData * data)
{

}

void CThumbnailFileSelection::AddSeparatorBar(const wxString &libelle, int &nbElement)
{
	CInfosSeparationBarExplorer * infosSeparationBar = new CInfosSeparationBarExplorer(themeThumbnail.themeSeparation);
	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetWidth(GetWindowWidth());
	int typeElement = TYPEMULTIPAGE;

	for (auto i = 0; i < photoVector.size();i++)
	{
		CImageVideoThumbnail * thumbnail = photoVector.at(i);
		infosSeparationBar->listElement.push_back((int)iconeList->GetNbElement());

		CThumbnailDataStorage * thumbnailData = new CThumbnailDataStorage(filename);
		//thumbnailData->SetStorage(nullptr);
		thumbnailData->SetNumPhotoId(i);
		thumbnailData->SetNumElement(i);
		thumbnailData->SetTypeElement(typeElement);
		thumbnailData->SetLibelle("Page : " + to_string(i + 1) + "/" + to_string(photoVector.size()));
		thumbnailData->SetTimePosition(thumbnail->timePosition);
		thumbnailData->SetBitmap(thumbnail->image);

		CIcone * pBitmapIcone = new CIcone();
		pBitmapIcone->SetNumElement(i);
		pBitmapIcone->ShowSelectButton(true);
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		iconeList->AddElement(pBitmapIcone);
	}

	if (photoVector.size() > 0)
		listSeparator.push_back(infosSeparationBar);
}

void CThumbnailFileSelection::InitTypeAffichage(const int &typeAffichage)
{
	threadDataProcess = false;

	//---------------------------------
	//Sauvegarde de l'état
	//---------------------------------

	EraseThumbnailList();

	for (CInfosSeparationBar * infosSeparationBar : listSeparator)
	{
		delete(infosSeparationBar);
	}

	listSeparator.clear();

	int i = 0;
	int typeLocal = typeAffichage;



	if (typeLocal == THUMB_SHOW_ALL)
	{
		wxString libellePhoto = "Select Page";// CLibResource::LoadStringFromResource(L"LBLALLPHOTO", 1);
		AddSeparatorBar(libellePhoto, i);
	}

	//m_lock.unlock();

	SetNbFiles(i);

	AfterSetList();

	thumbnailPos = 0;

	threadDataProcess = true;

	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();
	this->Refresh();
}

void CThumbnailFileSelection::Init(const wxString &filename, const int &typeAffichage)
{
	photoVector.clear();

	CLibPicture libPicture;
	libPicture.LoadAllVideoThumbnail(filename, &photoVector, true, true);

	if (photoVector.size() > 0)
	{
		if (noVscroll)
			SetListeFile();
		else
			InitTypeAffichage(typeAffichage);
	}
	processIdle = true;	
}

void CThumbnailFileSelection::SetListeFile()
{
	threadDataProcess = false;

	EraseThumbnailList();

	int i = 0;
	int x = 0;
	int y = 0;
	thumbnailPos = 0;
	int typeElement = TYPEMULTIPAGE;


	for (auto i = 0; i < photoVector.size(); i++)
	{
		CImageVideoThumbnail * thumbnail = photoVector.at(i);

		CThumbnailDataStorage * thumbnailData = new CThumbnailDataStorage(filename);
		//thumbnailData->SetStorage(nullptr);
		thumbnailData->SetNumPhotoId(i);
		thumbnailData->SetNumElement(i);
		thumbnailData->SetTypeElement(typeElement);
		thumbnailData->SetLibelle("Page : " + to_string(i + 1) + "/" + to_string(photoVector.size()));
		thumbnailData->SetTimePosition(thumbnail->timePosition);
		thumbnailData->SetBitmap(thumbnail->image);


		CIcone * pBitmapIcone = new CIcone();
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetWindowPos(x, y);

		iconeList->AddElement(pBitmapIcone);

		x += themeThumbnail.themeIcone.GetWidth();
		i++;

	}
	AfterSetList();
	threadDataProcess = true;
	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();
    processIdle = true;	
	this->Refresh();
}

vector<int> CThumbnailFileSelection::GetSelectItem()
{
	vector<int> listElement;
	int numElement = iconeList->GetNbElement();
	for (int i = 0; i < numElement; i++)
	{
		CIcone * pIcone = iconeList->GetElement(i);
		if (pIcone != nullptr)
		{
			if (pIcone->IsChecked())
				listElement.push_back(i);
		}
	}
	return listElement;
}

CIcone * CThumbnailFileSelection::FindIcone(const int &photoId)
{
    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
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

bool CThumbnailFileSelection::ItemCompWithVScrollFonct(int x, int y, CIcone * icone, CWindowMain * parent)   /* Définit une fonction. */
{
	wxRect rc = icone->GetPos();
	if ((rc.x < x && x < (rc.x + rc.width)) && (rc.y < y && y < (rc.height + rc.y)))
	{
		return true;
	}
	return false;
}

CIcone * CThumbnailFileSelection::FindElementWithVScroll(const int &xPos, const int &yPos)
{
	//int x = xPos;
	//int y = yPos;
	pItemCompFonct _pf = &ItemCompWithVScrollFonct;
	return iconeList->FindElement(xPos, yPos, &_pf, this);

	/*
    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * icone = iconeList->GetElement(i);
		if (icone != nullptr)
		{
			wxRect rc = icone->GetPos();
			if ((rc.x < x && x < (rc.x + rc.width)) && (rc.y < y && y < (rc.height + rc.y)))
			{
				return icone;
			}
		}
	}

	return nullptr;*/
}


CInfosSeparationBar * CThumbnailFileSelection::FindSeparatorElement(const int &xPos, const int &yPos)
{
    /*
	int x = xPos + posLargeur;
	int y = yPos + posHauteur;
	CInfosSeparationBar * element;

	InfosSeparationBarVector::iterator it;
	it = find_if(listSeparator.begin(), listSeparator.end(), CItemPosSeparationBar(xPos, yPos));
	if (it != listSeparator.end())
		element = *it;

	return element;
*/
	int x = xPos + posLargeur;
	int y = yPos + posHauteur;
	for (CInfosSeparationBar * separatorBar : listSeparator)
	{
		if (separatorBar != nullptr)
		{
			wxRect rc = separatorBar->GetPos();
			if ((rc.x < x && x < (rc.x + rc.width)) && (rc.y < y && y < (rc.height + rc.y)))
			{
				return separatorBar;
			}
		}
	}
	return nullptr;
	
}


void CThumbnailFileSelection::FindOtherElement(wxDC * dc, const int &x, const int &y)
{
	CInfosSeparationBar * separator = FindSeparatorElement(x, y);
	if (separator != nullptr)
	{
		CInfosSeparationBarExplorer * explorer = (CInfosSeparationBarExplorer *)separator;
		if (explorer != nullptr)
		{
			explorer->OnClick(x, y);
			//explorer->RenderIcone(dc);

            for (auto numElement : separator->listElement)		
			{
				CIcone * icone = iconeList->GetElement(numElement);
                if(icone != nullptr)
                {
                    if (explorer->GetSelected())
                        icone->SetChecked(true);
                    else
                        icone->SetChecked(false); 
                }

				//icone->RenderIcone(dc);
			}
		}
	}
}

void CThumbnailFileSelection::ResizeThumbnail()
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

		int elementByRow = (GetWindowWidth() + positionXOld) / themeThumbnail.themeIcone.GetWidth();
		if ((elementByRow * themeThumbnail.themeIcone.GetWidth()) <  (GetWindowWidth() + positionXOld))
			elementByRow++;

		if (nbElementByRow < elementByRow)
			nbElementByRow = elementByRow;
	}

	int controlWidth = nbElementByRow * themeThumbnail.themeIcone.GetWidth();

	for (CInfosSeparationBar * infosSeparationBar : listSeparator)
	{
		int nbElementEnY = 0;
		int nbElement = (int)infosSeparationBar->listElement.size();
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
			CIcone * pBitmapIcone = iconeList->GetElement(numElement);
            if(pBitmapIcone != nullptr)
            {
                pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
                pBitmapIcone->SetWindowPos(x, y);

                x += themeThumbnail.themeIcone.GetWidth(); nbElementX++;
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

bool CThumbnailFileSelection::ItemCompFonct(int xPos, int yPos, CIcone * icone, CWindowMain * parent)   /* Définit une fonction. */
{
	if (icone != nullptr && parent != nullptr)
	{
		CThumbnailFileSelection * thumbnail = (CThumbnailFileSelection *)parent;
		wxRect rc = icone->GetPos();
		int left = rc.x - thumbnail->posLargeur;
		int right = rc.x + rc.width - thumbnail->posLargeur;
		int top = rc.y - thumbnail->posHauteur;
		int bottom = rc.y + rc.height - thumbnail->posHauteur;
		if ((left < xPos && xPos < right) && (top < yPos && yPos < bottom))
		{
			return true;
		}
	}
	return false;
}

CIcone * CThumbnailFileSelection::FindElement(const int &xPos, const int &yPos)
{
	pItemCompFonct _pf = &ItemCompFonct;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
	/*
    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
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
	*/
}


void CThumbnailFileSelection::RenderIconeWithVScroll(wxDC * deviceContext)
{
	for (auto i = 0; i < listSeparator.size(); i++)
	{
		CInfosSeparationBar * infosSeparationBar = listSeparator.at(i);
		infosSeparationBar->Render(deviceContext, -posLargeur,-posHauteur);

		for (auto j = 0; j < infosSeparationBar->listElement.size(); j++)
		{
			int numElement = infosSeparationBar->listElement.at(j);
			CIcone * pBitmapIcone = iconeList->GetElement(numElement);
			if (pBitmapIcone != nullptr)
			{
				wxRect rc = pBitmapIcone->GetPos();

				//if visible
				int left = rc.x - posLargeur;
				int right = rc.x + rc.width - posLargeur;
				int top = rc.y - posHauteur;
				int bottom = rc.y + rc.height - posHauteur;

				if ((right > 0 && left < GetWindowWidth()) && (top < GetWindowHeight() && bottom > 0))
					RenderBitmap(deviceContext, pBitmapIcone, -posLargeur, -posHauteur);
				else
					pBitmapIcone->DestroyCache();
			}
		}
	}
}

void CThumbnailFileSelection::UpdateScrollWithVScroll()
{
	//bool update = false;
	int oldthumbnailSizeX = thumbnailSizeX;
	int oldthumbnailSizeY = thumbnailSizeY;

	thumbnailSizeX = 0;
	thumbnailSizeY = 0;

	for (CInfosSeparationBar * infosSeparationBar : listSeparator)
	{
		int nbElement = (int)infosSeparationBar->listElement.size();

		int nbElementByRow = (GetWindowWidth() + positionXOld) / themeThumbnail.themeIcone.GetWidth();

		if (nbElement > 0 && nbElementByRow == 0)
		{
			float value = (float)(GetWindowWidth() + positionXOld) / (float)themeThumbnail.themeIcone.GetWidth();
			if (value > 0)
				nbElementByRow = 1;
		}

		if (nbElementByRow > 0)
		{
			if (nbElementByRow < 1)
				nbElementByRow = 1;

			if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth() + positionXOld))
				nbElementByRow++;

			int nbElementEnY = (int)infosSeparationBar->listElement.size() / nbElementByRow;
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

    printf("CThumbnailFileSelection::UpdateScrollWithVScroll old %d %d \n", oldthumbnailSizeX, oldthumbnailSizeY);
    printf("CThumbnailFileSelection::UpdateScrollWithVScroll new %d %d \n", thumbnailSizeX, thumbnailSizeY);

	//bool refresh = false;
	if (iconeList->GetNbElement() >= 0)
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
			xRatio = (float)thumbnailSizeX / (float)oldthumbnailSizeX;

		if (oldthumbnailSizeY != 0)
			yRatio = (float)thumbnailSizeY / (float)oldthumbnailSizeY;

		float posX = (float)posLargeur * xRatio;
		float posY = (float)posHauteur * yRatio;


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
			size->x = posX;
			size->y = posY;
			evt.SetClientData(size);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}
		posLargeur = posX;
		posHauteur = posY;
	}

	this->Refresh();
}
