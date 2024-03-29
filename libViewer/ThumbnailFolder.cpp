﻿// ReSharper disable All
#include <header.h>
#include "ThumbnailFolder.h"
#include "MainWindow.h"
#include <LibResource.h>
#include <ThumbnailDataSQL.h>
#include <SqlFindPhotos.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarWnd.h>
#include <SqlPhotosWithoutThumbnail.h>
#include "TreatmentData.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <config_id.h>
#include <ParamInit.h>
#include "ThumbnailBuffer.h"
#include <RegardsConfigParam.h>
using namespace Regards::Viewer;
using namespace Regards::Sqlite;




CThumbnailFolder::CThumbnailFolder(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
                                   const bool& testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity)
{
	barseparationHeight = 40;
	widthThumbnail = 0;
	heightThumbnail = 0;
	preprocess_thumbnail = true;
}


CThumbnailFolder::~CThumbnailFolder(void)
{
	if (listSeparator != nullptr)
	{
		for (CInfosSeparationBar* infosSeparationBar : *listSeparator)
		{
			delete(infosSeparationBar);
		}

		listSeparator->clear();
		delete listSeparator;
	}
}

void CThumbnailFolder::OnPictureClick(CThumbnailData* data)
{
	CMainWindow* mainWindow = (CMainWindow*)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr && data != nullptr)
	{
		//mainWindow->PictureVideoClick(timePosition);
		wxCommandEvent evt(wxEVENT_ONPICTURECLICK);
		evt.SetExtraLong(data->GetNumPhotoId());
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

CInfosSeparationBarExplorer * CThumbnailFolder::AddSeparatorBar(PhotosVector * _pictures, CIconeList* iconeListLocal, const wxString& libelle, int& nbElement)
{
	CInfosSeparationBarExplorer* infosSeparationBar = new CInfosSeparationBarExplorer(themeThumbnail.themeSeparation);
	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetWidth(GetWindowWidth());
	infosSeparationBar->ShowExpandIcon(true);
	int local_nbElement = iconeListLocal->GetNbElement();
	tbb::concurrent_vector<int>* list = new tbb::concurrent_vector<int>();
	int size = _pictures->size();
    
	tbb::parallel_for(0, size, 1, [=](int i)
	{
		try
		{
			CPhotos photo = _pictures->at(i);

			std::vector<CIcone*>::iterator it = std::find_if(pIconeList.begin(), pIconeList.end(), [&](CIcone* e)
				{
					CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)e->GetData();
					return thumbnailData->GetNumPhotoId() == photo.GetId();

				});

			if (it != pIconeList.end())
			{
				CIcone* icone = (CIcone*)*it;
				CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)icone->GetData();
				thumbnailData->SetNumElement(local_nbElement + i);
				icone->SetNumElement(thumbnailData->GetNumElement());
				iconeListLocal->AddElement(icone);
			}
			else
				list->push_back(i);

		}
		catch(...)
		{

		}
	}
    );

	for (int i : *list)
	{
		CPhotos photo = _pictures->at(i);
		CThumbnailDataSQL* thumbnailData = new CThumbnailDataSQL(photo.GetPath(), testValidity, false);
		thumbnailData->SetNumPhotoId(photo.GetId());
		thumbnailData->SetNumElement(local_nbElement + i);

		auto pBitmapIcone = new CIcone();
		pBitmapIcone->ShowSelectButton(true);
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		iconeListLocal->AddElement(pBitmapIcone);
		pIconeList.push_back(pBitmapIcone);
	}

	iconeListLocal->SortById();

	for (auto i = 0; i < size; i++)
	{
		infosSeparationBar->listElement.push_back(local_nbElement + i);
	}

	nbElement += size;

	delete list;
	
	//if (size > 0)
	//	listSeparator.push_back(infosSeparationBar);
	return infosSeparationBar;
}


// Compares two intervals
// according to starting times.
bool CThumbnailFolder::compareFilename(CPhotos i1, CPhotos i2)
{
	return (i1.GetPath() < i2.GetPath());
}

void CThumbnailFolder::SortVectorByFilename(PhotosVector* vector)
{
	//std::sort(vector->begin(), vector->end(), compareFilename);
}

void CThumbnailFolder::InitTypeAffichage(const int& typeAffichage)
{
    std::vector<CIcone*> pIconeListToClean;
	CIconeList* iconeListLocal = new CIconeList();
	CIconeList* oldIconeList = iconeList;
	//---------------------------------
	//Sauvegarde de l'état
	//---------------------------------
	vector<CThumbnailData*> listSelectItem;
	threadDataProcess = false;
	GetSelectItem(listSelectItem);

	InfosSeparationBarVector* _listSeparator = new InfosSeparationBarVector();
	InfosSeparationBarVector* old = listSeparator;

	int i = 0;
	int typeLocal = typeAffichage;
	int size = CThumbnailBuffer::GetVectorSize();

	if (typeLocal == SHOW_ALL)
	{
		CTreatmentDataFolder dataYear;
		dataYear.MainTreatment(_listSeparator, iconeListLocal, this, i);
	}
	else if (typeLocal == SHOW_BYYEAR)
	{
		CTreatmentDataYear dataYear;
		dataYear.MainTreatment(_listSeparator, iconeListLocal, this, i);
	}
	else if (typeLocal == SHOW_BYMONTH)
	{
		CTreatmentDataMonth dataMonth;
		dataMonth.MainTreatment(_listSeparator, iconeListLocal, this, i);
	}
	else if (typeLocal == SHOW_BYLOCALISATION)
	{
		CTreatmentDataLocalisation dataLocalisation;
		dataLocalisation.MainTreatment(_listSeparator, iconeListLocal, this, i);
	}
	else if (typeLocal == SHOW_BYDAY)
	{
		CTreatmentDataDay dataDay;
		dataDay.MainTreatment(_listSeparator, iconeListLocal, this, i);
	}


	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
		config->SetTypeAffichage(typeAffichage);
	}

	iconeList = iconeListLocal;
	listSeparator = _listSeparator;

	//------------------------------------------------------------------
	//Cleaning old Element
	//------------------------------------------------------------------
	if (old != nullptr)
	{
		for (CInfosSeparationBar* infosSeparationBar : *old)
		{
			CInfosSeparationBarExplorer* infosExplorerOld = (CInfosSeparationBarExplorer*)infosSeparationBar;
			for (int i = 0; i < listSeparator->size(); i++)
			{
				CInfosSeparationBarExplorer* infosSeparationNew = (CInfosSeparationBarExplorer*)listSeparator->at(i);
				if (infosSeparationNew->GetLongTitle() == infosExplorerOld->GetLongTitle())
				{
					infosSeparationNew->SetSelected(infosExplorerOld->GetSelected());
					infosSeparationNew->SetShow(infosExplorerOld->GetShow());
					break;
				}
			}

			delete(infosSeparationBar);
		}

		old->clear();
		delete old;
	}

	
    /*
	if (oldIconeList != nullptr)
	{
		delete oldIconeList;
		oldIconeList = nullptr;
	}
    */

	nbElementInIconeList = iconeList->GetNbElement();
    
	GenerateCleanupListFile(pIconeListToClean);

	EraseThumbnailList(oldIconeList);
	EraseIconeList(pIconeListToClean);

	//---------------------------------
	//Application de l'état
	//---------------------------------

	if (listSelectItem.size() > 0)
	{
		for (CThumbnailData* data : listSelectItem)
		{
			int itemId = GetNumItemById(data->GetNumPhotoId());
			CIcone *  icone = iconeList->GetElement(itemId);
			if (icone != nullptr)
			{
				icone->SetChecked(true);
				icone->SetSelected(true);
			}
		}
	}

	AfterSetList();

	thumbnailPos = 0;

	threadDataProcess = true;

	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();

	needToRefresh = true;
}

void CThumbnailFolder::Init(const int& typeAffichage)
{
	if (noVscroll)
		SetListeFile();
	else
		InitTypeAffichage(typeAffichage);

}

void CThumbnailFolder::SetListeFile()
{
	std::vector<CIcone*> pIconeListToClean;
	CIconeList* iconeListLocal = new CIconeList();
	threadDataProcess = false;
	thumbnailPos = 0;
	int size = CThumbnailBuffer::GetVectorSize();
	tbb::concurrent_vector<int>* list = new tbb::concurrent_vector<int>();

	tbb::parallel_for(0, size, 1, [=](int i)      
	{
		try
		{
			CPhotos fileEntry = CThumbnailBuffer::GetVectorValue(i);

			std::vector<CIcone*>::iterator it = std::find_if(pIconeList.begin(), pIconeList.end(), [&](CIcone* e)
				{
					CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)e->GetData();
					return thumbnailData->GetFilename() == fileEntry.GetPath();

				});

			if (it != pIconeList.end())
			{
				CIcone* icone = (CIcone*)*it;
				CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)icone->GetData();
				thumbnailData->SetNumPhotoId(fileEntry.GetId());
				thumbnailData->SetNumElement(i);
				icone->SetNumElement(thumbnailData->GetNumElement());
				icone->SetWindowPos(themeThumbnail.themeIcone.GetWidth() * i, 0);
				iconeListLocal->AddElement(icone);
			}
			else
				list->push_back(i);
		}
		catch(...)
		{

		}
	}
    );


	for (int i : *list)
	{
		try
		{
			CPhotos fileEntry = CThumbnailBuffer::GetVectorValue(i);

			wxString filename = fileEntry.GetPath();
			CThumbnailDataSQL* thumbnailData = new CThumbnailDataSQL(filename, false, false);
			thumbnailData->SetNumPhotoId(fileEntry.GetId());
			thumbnailData->SetNumElement(i);


			auto pBitmapIcone = new CIcone();
			pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(themeThumbnail.themeIcone.GetWidth() * i, 0);

			iconeListLocal->AddElement(pBitmapIcone);
			pIconeList.push_back(pBitmapIcone);
		}
		catch(...)
		{

		}
	}

	delete list;

	iconeListLocal->SortById();

	CIconeList* oldIconeList = iconeList;
	iconeList = iconeListLocal;

	nbElementInIconeList = iconeList->GetNbElement();
    
	GenerateCleanupListFile(pIconeListToClean);

	EraseThumbnailList(oldIconeList);
	EraseIconeList(pIconeListToClean);
	//----------------------------------------------------------
	// 
	//EraseThumbnailList(oldIconeList);

	AfterSetList();

	threadDataProcess = true;
	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();

	needToRefresh = true;
}


bool CThumbnailFolder::ItemCompFonctWithVScroll(int x, int y, CIcone *  icone, CWindowMain* parent)
/* Définit une fonction. */
{
	if (icone != nullptr && parent != nullptr)
	{
		wxRect rc = icone->GetPos();
		if ((rc.x < x && x < (rc.x + rc.width)) && (rc.y < y && y < (rc.height + rc.y)))
		{
			return true;
		}
	}
	return false;
}

CIcone *  CThumbnailFolder::FindElementWithVScroll(const int& xPos, const int& yPos)
{
	pItemCompFonct _pf = &ItemCompFonctWithVScroll;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
}


CInfosSeparationBar* CThumbnailFolder::FindSeparatorElement(const int& xPos, const int& yPos)
{
	int x = xPos + posLargeur;
	int y = yPos + posHauteur;

	if (listSeparator == nullptr)
		return nullptr;

	for (CInfosSeparationBar* separatorBar : *listSeparator)
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


void CThumbnailFolder::FindOtherElement(wxDC* dc, const int& x, const int& y)
{
	bool updateShowFile = false;
	CInfosSeparationBar* separator = FindSeparatorElement(x, y);
	if (separator != nullptr)
	{
		CInfosSeparationBarExplorer* explorer = (CInfosSeparationBarExplorer*)separator;
		if (explorer != nullptr)
		{
			bool status = explorer->GetShow();
			explorer->OnClick(x, y);
			//explorer->RenderIcone(dc);

			for (auto numElement : separator->listElement)
			{
				CIcone *  icone = iconeList->GetElement(numElement);
				if (icone != nullptr)
				{
					if (explorer->GetSelected())
						icone->SetChecked(true);
					else
						icone->SetChecked(false);
				}

				//icone->RenderIcone(dc);
			}

			if (status != explorer->GetShow())
				updateThumbnail = true;
		}
	}
}

void CThumbnailFolder::ResizeThumbnail()
{
	int x = 0;
	int y = 0;

	int nbElementX = 0;
	int nbElementY = 0;

	int nbElementByRow = 0;

	if (listSeparator == nullptr)
		return;

	//Calcul du width max
	for (auto i = 0; i < listSeparator->size(); i++)
	{
		//int nbElement = infosSeparationBar->listElement.size();

		int elementByRow = (GetWindowWidth()) / themeThumbnail.themeIcone.GetWidth();
		if ((elementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth()))
			elementByRow++;

		if (nbElementByRow < elementByRow)
			nbElementByRow = elementByRow;
	}

	int controlWidth = nbElementByRow * themeThumbnail.themeIcone.GetWidth();

	for (CInfosSeparationBar* infosSeparationBar : *listSeparator)
	{
		CInfosSeparationBarExplorer* infosExplorer = (CInfosSeparationBarExplorer*)infosSeparationBar;
		int nbElement_localX = 0;
		int nbElement_localY = 0;

		infosSeparationBar->SetWidth(controlWidth);
		infosSeparationBar->SetWindowPos(x, y);

		y += infosSeparationBar->GetHeight();

		if (infosExplorer->GetShow())
		{
			for (auto numElement : infosSeparationBar->listElement)
			{
				CIcone *  pBitmapIcone = iconeList->GetElement(numElement);
				if (pBitmapIcone != nullptr)
				{
					pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
					pBitmapIcone->SetWindowPos(x, y);
					pBitmapIcone->SetVisibility(true);
					x += themeThumbnail.themeIcone.GetWidth();

					nbElement_localX++;
					nbElementX++;
					if (nbElementX == nbElementByRow)
					{
						nbElementX = 0;
						x = 0;
						nbElementY++;
						nbElement_localY++;
						y += themeThumbnail.themeIcone.GetHeight();
					}
				}
			}

			if (nbElementX != 0)
			{
				nbElementX = 0;
				x = 0;
				nbElementY++;
				nbElement_localY++;
				y += themeThumbnail.themeIcone.GetHeight();
			}


			infosSeparationBar->SetNbElementX(nbElement_localX);
			infosSeparationBar->SetNbElementY(nbElement_localY);
		}
		else
		{
			for (auto numElement : infosSeparationBar->listElement)
			{
				CIcone *  pBitmapIcone = iconeList->GetElement(numElement);
				if (pBitmapIcone != nullptr)
				{
					pBitmapIcone->SetVisibility(false);
				}
			}
		}

	}

	widthThumbnail = GetWindowWidth();
	heightThumbnail = GetWindowHeight();

	UpdateScroll();
}

bool CThumbnailFolder::ItemCompFonct(int xPos, int yPos, CIcone *  icone, CWindowMain* parent) /* Définit une fonction. */
{
	if (icone->GetVisibility())
	{
		if (icone != nullptr && parent != nullptr)
		{
			auto folder = (CThumbnailFolder*)parent;
			wxRect rc = icone->GetPos();
			int left = rc.x - folder->posLargeur;
			int right = rc.x + rc.width - folder->posLargeur;
			int top = rc.y - folder->posHauteur;
			int bottom = rc.y + rc.height - folder->posHauteur;
			if ((left < xPos && xPos < right) && (top < yPos && yPos < bottom))
			{
				return true;
			}
		}
	}
	return false;
}

CIcone *  CThumbnailFolder::FindElement(const int& xPos, const int& yPos)
{
	pItemCompFonct _pf = &ItemCompFonct;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
}


void CThumbnailFolder::RenderIconeWithVScroll(wxDC* deviceContext)
{
	if (listSeparator == nullptr)
		return;

	if (updateThumbnail)
		ResizeThumbnail();
	updateThumbnail = false;

	for (auto i = 0; i < listSeparator->size(); i++)
	{

		CInfosSeparationBarExplorer * infosSeparationBar = (CInfosSeparationBarExplorer *)listSeparator->at(i);

		if (infosSeparationBar != nullptr)
		{
            bool start = false;
            infosSeparationBar->Render(deviceContext, -posLargeur, -posHauteur);

            if (infosSeparationBar->GetShow())
            {
                
                for (auto j = 0; j < infosSeparationBar->listElement.size(); j++)
                {
                    int numElement = infosSeparationBar->listElement.at(j);
                    CIcone *  pBitmapIcone = iconeList->GetElement(numElement);
                    if (pBitmapIcone != nullptr)
                    {
                        wxRect rc = pBitmapIcone->GetPos();
                        //if visible
                        int left = rc.x - posLargeur;
                        int right = rc.x + rc.width - posLargeur;
                        int top = rc.y - posHauteur;
                        int bottom = rc.y + rc.height - posHauteur;

                        if ((right > 0 && left < GetWindowWidth()) && (top < GetWindowHeight() && bottom > 0))
                        {
                            if (!start)
                                start = true;

                            RenderBitmap(deviceContext, pBitmapIcone, -posLargeur, -posHauteur);

                        }
                        else if (start)
                            break;
                    }
                }
            }
		}
	}
}

void CThumbnailFolder::UpdateScrollWithVScroll()
{
	//bool update = false;
	int oldthumbnailSizeX = thumbnailSizeX;
	int oldthumbnailSizeY = thumbnailSizeY;

	thumbnailSizeX = 0;
	thumbnailSizeY = 0;


	if (listSeparator == nullptr)
		return;

	for (CInfosSeparationBar* infosSeparationBar : *listSeparator)
	{
        CInfosSeparationBarExplorer * infosSeparationBarExplorer = (CInfosSeparationBarExplorer *)infosSeparationBar;
        if (infosSeparationBarExplorer->GetShow())
        {
            int nbElement = (int)infosSeparationBar->listElement.size();

            int nbElementByRow = (GetWindowWidth()) / themeThumbnail.themeIcone.GetWidth();

            if (nbElement > 0 && nbElementByRow == 0)
            {
                float value = (float)(GetWindowWidth()) / (float)themeThumbnail.themeIcone.GetWidth();
                if (value > 0)
                    nbElementByRow = 1;
            }

            if (nbElementByRow > 0)
            {
                if (nbElementByRow < 1)
                    nbElementByRow = 1;

                if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth()))
                    nbElementByRow++;

                int nbElementEnY = (int)infosSeparationBar->listElement.size() / nbElementByRow;
                if (nbElementEnY * nbElementByRow < infosSeparationBar->listElement.size())
                    nbElementEnY++;

                infosSeparationBar->SetNbElementY(nbElementEnY);

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
        else
        {
            int sizeX = infosSeparationBar->GetWidth();
            if (sizeX > thumbnailSizeX)
                    thumbnailSizeX = infosSeparationBar->GetWidth();
            thumbnailSizeY += infosSeparationBar->GetHeight();
        }
	}

	//printf("CThumbnailFolder::UpdateScrollWithVScroll old %d %d \n", oldthumbnailSizeX, oldthumbnailSizeY);
	//printf("CThumbnailFolder::UpdateScrollWithVScroll new %d %d \n", thumbnailSizeX, thumbnailSizeY);

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
			xRatio = (float)thumbnailSizeX / (float)oldthumbnailSizeX;

		if (oldthumbnailSizeY != 0)
			yRatio = (float)thumbnailSizeY / (float)oldthumbnailSizeY;

		float posX = (float)posLargeur * xRatio;
		float posY = (float)posHauteur * yRatio;

		wxWindow* parent = this->GetParent();

		if (parent != nullptr)
		{
			CControlSize* controlSize = new CControlSize();
			wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
			controlSize->controlWidth = thumbnailSizeX;
			controlSize->controlHeight = thumbnailSizeY;
			evt.SetClientData(controlSize);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}

		if (parent != nullptr)
		{
			wxSize* size = new wxSize();
			wxCommandEvent evt(wxEVENT_SETPOSITION);
			size->x = static_cast<int>(posX);
			size->y = static_cast<int>(posY);
			evt.SetClientData(size);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}

		posLargeur = posX;
		posHauteur = posY;
	}

	needToRefresh = true;
}
