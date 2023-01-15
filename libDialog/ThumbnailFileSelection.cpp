// ReSharper disable All
#include <header.h>
#include "ThumbnailFileSelection.h"
#include <LibResource.h>
#include <ThumbnailDataStorage.h>
#include <libPicture.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarWnd.h>
#include <picture_utility.h>
#include <ImageVideoThumbnail.h>
using namespace Regards::Scanner;
using namespace Regards::Sqlite;
using namespace Regards::Picture;

CThumbnailFileSelection::CThumbnailFileSelection(wxWindow* parent, const wxWindowID idCTreeWithScrollbarInterface,
                                                 const CThemeThumbnail& themeThumbnail,
                                                 const bool& testValidity)
	: CThumbnailVertical(parent, idCTreeWithScrollbarInterface, themeThumbnail, testValidity)
{
	barseparationHeight = 40;
	widthThumbnail = 0;
	heightThumbnail = 0;
	moveOnPaint = false;
}


CThumbnailFileSelection::~CThumbnailFileSelection(void)
{
}

void CThumbnailFileSelection::OnPictureClick(CThumbnailData* data)
{
}

void CThumbnailFileSelection::AddSeparatorBar(CIconeList* iconeListLocal, const wxString& libelle, int& nbElement)
{
	auto infosSeparationBar = new CInfosSeparationBarExplorer(themeThumbnail.themeSeparation);
	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetWidth(GetWindowWidth());
	int typeElement = TYPEMULTIPAGE;

	for (auto i = 0; i < photoVector.size(); i++)
	{
		CImageVideoThumbnail* thumbnail = photoVector.at(i);
		infosSeparationBar->listElement.push_back(i);

		auto thumbnailData = new CThumbnailDataStorage(filename);
		//thumbnailData->SetStorage(nullptr);
		thumbnailData->SetNumPhotoId(i);
		thumbnailData->SetNumElement(i);
		thumbnailData->SetTypeElement(typeElement);
		thumbnailData->SetLibelle("Page : " + to_string(i + 1) + "/" + to_string(photoVector.size()));
		thumbnailData->SetTimePosition(thumbnail->timePosition);

		if (!thumbnail->image.IsOk())
		{
			thumbnail->image.LoadFile(CPictureUtility::GetPhotoCancel(), wxBITMAP_TYPE_ANY);
		}

		thumbnailData->SetBitmap(thumbnail->image);

		auto pBitmapIcone = new CIcone();
		pBitmapIcone->SetNumElement(i);
		pBitmapIcone->ShowSelectButton(true);
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		iconeListLocal->AddElement(pBitmapIcone);
	}

	if (photoVector.size() > 0)
		listSeparator.push_back(infosSeparationBar);

	nbElement = photoVector.size(); // NOLINT(clang-diagnostic-shorten-64-to-32)
}

void CThumbnailFileSelection::InitTypeAffichage(const int& typeAffichage)
{
	auto iconeListLocal = new CIconeList();
	threadDataProcess = false;
	//---------------------------------
	//Sauvegarde de l'état
	//---------------------------------
	for (CInfosSeparationBar* infosSeparationBar : listSeparator)
	{
		delete(infosSeparationBar);
	}

	listSeparator.clear();

	int i = 0;
	int typeLocal = typeAffichage;


	if (typeLocal == THUMB_SHOW_ALL)
	{
		wxString libellePhoto = CLibResource::LoadStringFromResource(L"LBLSELECTPAGE", 1);
		AddSeparatorBar(iconeListLocal, libellePhoto, i);
	}


	lockIconeList.lock();
	CIconeList* oldIconeList = iconeList;
	iconeList = iconeListLocal;
	lockIconeList.unlock();

	nbElementInIconeList = iconeList->GetNbElement();

	EraseThumbnailList(oldIconeList);

	AfterSetList();

	thumbnailPos = 0;

	threadDataProcess = true;

	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();
	needToRefresh = true;
}

void CThumbnailFileSelection::Init(const wxString& filename, const int& typeAffichage)
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
	auto iconeListLocal = new CIconeList();
	threadDataProcess = false;
	int x = 0;
	int y = 0;
	thumbnailPos = 0;
	int typeElement = TYPEMULTIPAGE;


	for (auto i = 0; i < photoVector.size(); i++)
	{
		CImageVideoThumbnail* thumbnail = photoVector.at(i);

		auto thumbnailData = new CThumbnailDataStorage(filename);
		//thumbnailData->SetStorage(nullptr);
		thumbnailData->SetNumPhotoId(i);
		thumbnailData->SetNumElement(i);
		thumbnailData->SetTypeElement(typeElement);
		thumbnailData->SetLibelle("Page : " + to_string(i + 1) + "/" + to_string(photoVector.size()));
		thumbnailData->SetTimePosition(thumbnail->timePosition);

		if (!thumbnail->image.IsOk())
		{
			thumbnail->image.LoadFile(CPictureUtility::GetPhotoCancel(), wxBITMAP_TYPE_ANY);
		}

		thumbnailData->SetBitmap(thumbnail->image);
		auto pBitmapIcone = new CIcone();
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetWindowPos(x, y);
		iconeListLocal->AddElement(pBitmapIcone);
		x += themeThumbnail.themeIcone.GetWidth();
	}


	lockIconeList.lock();
	CIconeList* oldIconeList = iconeList;
	iconeList = iconeListLocal;
	lockIconeList.unlock();

	nbElementInIconeList = iconeList->GetNbElement();

	EraseThumbnailList(oldIconeList);

	AfterSetList();

	threadDataProcess = true;

	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();
	processIdle = true;
	needToRefresh = true;
}

vector<int> CThumbnailFileSelection::GetSelectItem()
{
	vector<int> listElement;
	for (int i = 0; i < nbElementInIconeList; i++)
	{
		CIcone* pIcone = iconeList->GetElement(i);
		if (pIcone != nullptr)
		{
			if (pIcone->IsChecked())
				listElement.push_back(i);
		}
	}
	return listElement;
}

bool CThumbnailFileSelection::ItemCompWithVScrollFonct(int x, int y, CIcone* icone, CWindowMain* parent)
/* Définit une fonction. */
{
	wxRect rc = icone->GetPos();
	if ((rc.x < x && x < (rc.x + rc.width)) && (rc.y < y && y < (rc.height + rc.y)))
	{
		return true;
	}
	return false;
}

CIcone* CThumbnailFileSelection::FindElementWithVScroll(const int& xPos, const int& yPos)
{
	pItemCompFonct _pf = &ItemCompWithVScrollFonct;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
}


CInfosSeparationBar* CThumbnailFileSelection::FindSeparatorElement(const int& xPos, const int& yPos)
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


void CThumbnailFileSelection::FindOtherElement(wxDC* dc, const int& x, const int& y)
{
	CInfosSeparationBar* separator = FindSeparatorElement(x, y);
	if (separator != nullptr)
	{
		auto explorer = static_cast<CInfosSeparationBarExplorer*>(separator);
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

		int elementByRow = (GetWindowWidth()) / themeThumbnail.themeIcone.GetWidth();
		if ((elementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth()))
			elementByRow++;

		if (nbElementByRow < elementByRow)
			nbElementByRow = elementByRow;
	}

	int controlWidth = nbElementByRow * themeThumbnail.themeIcone.GetWidth();

	for (CInfosSeparationBar* infosSeparationBar : listSeparator)
	{
		//int nbElement = static_cast<int>(infosSeparationBar->listElement.size());

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

bool CThumbnailFileSelection::ItemCompFonct(int xPos, int yPos, CIcone* icone, CWindowMain* parent)
/* Définit une fonction. */
{
	if (icone != nullptr && parent != nullptr)
	{
		auto thumbnail = static_cast<CThumbnailFileSelection*>(parent);
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

CIcone* CThumbnailFileSelection::FindElement(const int& xPos, const int& yPos)
{
	pItemCompFonct _pf = &ItemCompFonct;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
}


void CThumbnailFileSelection::RenderIconeWithVScroll(wxDC* deviceContext)
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
				int left = rc.x - posLargeur;
				int right = rc.x + rc.width - posLargeur;
				int top = rc.y - posHauteur;
				int bottom = rc.y + rc.height - posHauteur;

				if ((right > 0 && left < GetWindowWidth()) && (top < GetWindowHeight() && bottom > 0))
					RenderBitmap(deviceContext, pBitmapIcone, -posLargeur, -posHauteur);
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

	printf("CThumbnailFileSelection::UpdateScrollWithVScroll old %d %d \n", oldthumbnailSizeX, oldthumbnailSizeY);
	printf("CThumbnailFileSelection::UpdateScrollWithVScroll new %d %d \n", thumbnailSizeX, thumbnailSizeY);

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
