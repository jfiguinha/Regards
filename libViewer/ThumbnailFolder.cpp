#include <header.h>
#include "ThumbnailFolder.h"
#include "MainWindow.h"
#include <LibResource.h>
#include <ThumbnailDataSQL.h>
#include <SqlFindPhotos.h>
#include <SqlFindYear.h>
#include <SqlFindMonth.h>
#include <SqlFindDay.h>
#include <SqlFindLocalisation.h>
#include <ConvertUtility.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarWnd.h>
#include <SqlPhotosWithoutThumbnail.h>
#include "TreatmentData.h"
using namespace Regards::Viewer;
using namespace Regards::Sqlite;


CThumbnailFolder::CThumbnailFolder(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail, const bool& testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity)
{

	barseparationHeight = 40;
	widthThumbnail = 0;
	heightThumbnail = 0;
}


CThumbnailFolder::~CThumbnailFolder(void)
{

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

void CThumbnailFolder::AddSeparatorBar(const wxString& libelle, PhotosVector* photoVector, int& nbElement)
{
	CInfosSeparationBarExplorer* infosSeparationBar = new CInfosSeparationBarExplorer(themeThumbnail.themeSeparation);
	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetWidth(GetWindowWidth());

	for (auto i = 0; i < photoVector->size(); i++)
	{
		CPhotos photo = photoVector->at(i);
		infosSeparationBar->listElement.push_back((int)iconeList->GetNbElement());

		CThumbnailDataSQL* thumbnailData = new CThumbnailDataSQL(photo.GetPath(), testValidity);
		thumbnailData->SetNumPhotoId(photo.GetId());
		thumbnailData->SetNumElement(nbElement++);

		CIcone* pBitmapIcone = new CIcone();
		pBitmapIcone->ShowSelectButton(true);
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		iconeList->AddElement(pBitmapIcone);
	}

	if (photoVector->size() > 0)
		listSeparator.push_back(infosSeparationBar);
}


void CThumbnailFolder::InitTypeAffichage(PhotosVector* photoVector, const int& typeAffichage)
{
	threadDataProcess = false;
	newPhotosVectorList.clear();
	//---------------------------------
	//Sauvegarde de l'état
	//---------------------------------

	//int numPhotoIdActif = 0;
	//int numPhotoIdSelect = 0;

	vector<CThumbnailData*> listSelectItem;

	GetSelectItem(listSelectItem);
	EraseThumbnailList();

	for (CInfosSeparationBar* infosSeparationBar : listSeparator)
	{
		delete(infosSeparationBar);
	}

	listSeparator.clear();

	int i = 0;
	int typeLocal = typeAffichage;

	if (typeLocal == SHOW_ALL)
	{
		
		wxString libellePhoto = CLibResource::LoadStringFromResource(L"LBLALLPHOTO", 1);
		AddSeparatorBar(libellePhoto, photoVector, i);
		if (photoVector->size() > 0)
		{
			newPhotosVectorList.reserve(photoVector->size());
			copy(photoVector->begin(), photoVector->end(), back_inserter(newPhotosVectorList));
		}

	}
	else if (typeLocal == SHOW_BYYEAR)
	{
		CTreatmentDataYear dataYear;
		dataYear.MainTreatment(photoVector, &newPhotosVectorList, this, i);
	}
	else if (typeLocal == SHOW_BYMONTH)
	{
		CTreatmentDataMonth dataMonth;
		dataMonth.MainTreatment(photoVector, &newPhotosVectorList, this, i);
	}
	else if (typeLocal == SHOW_BYLOCALISATION)
	{
		CTreatmentDataLocalisation dataLocalisation;
		dataLocalisation.MainTreatment(photoVector, &newPhotosVectorList, this, i);
	}
	else if (typeLocal == SHOW_BYDAY)
	{
		CTreatmentDataDay dataDay;
		dataDay.MainTreatment(photoVector, &newPhotosVectorList, this, i);
	}

	//m_lock.unlock();

	SetNbFiles(i);

	//---------------------------------
	//Application de l'tat
	//---------------------------------

	if (listSelectItem.size() > 0)
	{
		for (CThumbnailData* data : listSelectItem)
		{
			CIcone* icone = FindIcone(data->GetNumPhotoId());
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

	CMainWindow* mainWindow = (CMainWindow*)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_SETLISTPICTURE);
		evt.SetClientData(&newPhotosVectorList);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}

	this->Refresh();
}

void CThumbnailFolder::Init(const int& typeAffichage)
{
	photoVector.clear();

	CSqlFindPhotos sqlFindPhotos;
	sqlFindPhotos.SearchPhotosByCriteria(&photoVector);

	CSqlPhotosWithoutThumbnail sqlPhoto;
	sqlPhoto.GeneratePhotoList();

	if (noVscroll)
		SetListeFile(&photoVector);
	else
		InitTypeAffichage(&photoVector, typeAffichage);

	processIdle = true;
}

void CThumbnailFolder::SetListeFile(PhotosVector* photoVector)
{
	threadDataProcess = false;

	EraseThumbnailList();

	int i = 0;
	int x = 0;
	int y = 0;
	thumbnailPos = 0;

	for (CPhotos fileEntry : *photoVector)
	{
		wxString filename = fileEntry.GetPath();
		CThumbnailDataSQL* thumbnailData = new CThumbnailDataSQL(filename, testValidity);
		thumbnailData->SetNumPhotoId(fileEntry.GetId());
		thumbnailData->SetNumElement(i);


		CIcone* pBitmapIcone = new CIcone();
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

CIcone* CThumbnailFolder::FindIcone(const int& photoId)
{
	int numElement = iconeList->GetNbElement();
	for (int i = 0; i < numElement; i++)
	{
		CIcone* icone = iconeList->GetElement(i);
		if (icone != nullptr)
		{
			CThumbnailData* data = icone->GetData();
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

bool CThumbnailFolder::ItemCompFonctWithVScroll(int x, int y, CIcone* icone, CWindowMain* parent)   /* Définit une fonction. */
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

CIcone* CThumbnailFolder::FindElementWithVScroll(const int& xPos, const int& yPos)
{
	pItemCompFonct _pf = &ItemCompFonctWithVScroll;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
}


CInfosSeparationBar* CThumbnailFolder::FindSeparatorElement(const int& xPos, const int& yPos)
{
	int x = xPos + posLargeur;
	int y = yPos + posHauteur;

	for (CInfosSeparationBar* separatorBar : listSeparator)
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
	CInfosSeparationBar* separator = FindSeparatorElement(x, y);
	if (separator != nullptr)
	{
		CInfosSeparationBarExplorer* explorer = (CInfosSeparationBarExplorer*)separator;
		if (explorer != nullptr)
		{
			explorer->OnClick(x, y);
			//explorer->RenderIcone(dc);

			for (auto numElement : separator->listElement)
			{
				CIcone* icone = iconeList->GetElement(numElement);
				if (icone != nullptr)
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

void CThumbnailFolder::ResizeThumbnail()
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
			CIcone* pBitmapIcone = iconeList->GetElement(numElement);
			if (pBitmapIcone != nullptr)
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

bool CThumbnailFolder::ItemCompFonct(int xPos, int yPos, CIcone* icone, CWindowMain* parent)   /* Définit une fonction. */
{
	if (icone != nullptr && parent != nullptr)
	{
		CThumbnailFolder* folder = (CThumbnailFolder*)parent;
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
	return false;
}

CIcone* CThumbnailFolder::FindElement(const int& xPos, const int& yPos)
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


void CThumbnailFolder::RenderIconeWithVScroll(wxDC* deviceContext)
{

	for (auto i = 0; i < listSeparator.size(); i++)
	{
		CInfosSeparationBar* infosSeparationBar = listSeparator.at(i);
		infosSeparationBar->Render(deviceContext, -posLargeur, -posHauteur);

		for (auto j = 0; j < infosSeparationBar->listElement.size(); j++)
		{
			int numElement = infosSeparationBar->listElement.at(j);
			CIcone* pBitmapIcone = iconeList->GetElement(numElement);
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

void CThumbnailFolder::UpdateScrollWithVScroll()
{
	//bool update = false;
	int oldthumbnailSizeX = thumbnailSizeX;
	int oldthumbnailSizeY = thumbnailSizeY;

	thumbnailSizeX = 0;
	thumbnailSizeY = 0;

	for (CInfosSeparationBar* infosSeparationBar : listSeparator)
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

	//printf("CThumbnailFolder::UpdateScrollWithVScroll old %d %d \n", oldthumbnailSizeX, oldthumbnailSizeY);
	//printf("CThumbnailFolder::UpdateScrollWithVScroll new %d %d \n", thumbnailSizeX, thumbnailSizeY);

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