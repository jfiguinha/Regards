// ReSharper disable All
#include <header.h>
#include "ThumbnailFolder.h"
#include "MainWindow.h"
#include <ThumbnailDataSQL.h>
#include <ScrollbarWnd.h>
#include "TreatmentData.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <config_id.h>
#include <ParamInit.h>
#include "ThumbnailBuffer.h"
#include <RegardsConfigParam.h>
#include <wx/progdlg.h>
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


void CThumbnailFolder::OnPictureClick(const int& numPhotoId)
{
	CMainWindow* mainWindow = (CMainWindow*)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		//mainWindow->PictureVideoClick(timePosition);
		wxCommandEvent evt(wxEVENT_ONPICTURECLICK);
		evt.SetExtraLong(numPhotoId);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

std::unique_ptr<CInfosSeparationBarExplorer> CThumbnailFolder::AddSeparatorBar(PhotosVector* _pictures, CIconeList* iconeListLocal, const wxString& libelle, int& nbElement)
{
	auto infosSeparationBar = std::make_unique<CInfosSeparationBarExplorer>(themeThumbnail.themeSeparation);
	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetWidth(GetWindowWidth());
	infosSeparationBar->ShowExpandIcon(true);
	int local_nbElement = nbElement;// iconeListLocal->GetNbElement();

	int size = _pictures->size();

	if (needFindNewItem)
	{

		tbb::parallel_for(0, size, 1, [=](int i)
			//for (int i = 0; i < size; i++)
			{
				auto& photo = _pictures->at(i);
				wxString filename = photo.GetPath();
				bool find = iconeList->IfElementExistByFilename(photo.GetPath());
				if (!find)
				{
					auto thumbnailData = new CThumbnailDataSQL(photo.GetPath(), testValidity, false);
					thumbnailData->SetNumPhotoId(photo.GetId());
					thumbnailData->SetNumElement(local_nbElement + i);

					auto pBitmapIcone = new CIcone(thumbnailData);
					pBitmapIcone->ShowSelectButton(true);
					pBitmapIcone->SetNumElement(local_nbElement + i);
					pBitmapIcone->SetFilename(photo.GetPath());
					pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
					iconeListLocal->AddElement(pBitmapIcone);
				}
				else
				{
					CIcone* icone = iconeList->FindElementByFilename(photo.GetPath());
					if (icone != nullptr)
					{
						icone->SetNumElement(local_nbElement + i);
						auto data = static_cast<CThumbnailDataSQL*>(icone->GetPtData());
						if (data != nullptr)
						{
							data->SetNumElement(local_nbElement + i);
							icone->SetNumElement(data->GetNumElement());
						}

					}
				}
				//}
			});
	}
	else
	{
		tbb::parallel_for(0, size, 1, [=](int i)
			{
				auto& photo = _pictures->at(i);
				CIcone* icone = iconeList->FindElementByFilename(photo.GetPath());
				if (icone != nullptr)
				{
					icone->SetNumElement(local_nbElement + i);
					auto data = static_cast<CThumbnailDataSQL*>(icone->GetPtData());
					if (data != nullptr)
					{
						data->SetNumElement(local_nbElement + i);
						icone->SetNumElement(data->GetNumElement());
					}

				}
			});
	}


	iconeListLocal->SortById();

	for (auto i = 0; i < size; i++)
	{
		infosSeparationBar->listElement.push_back(local_nbElement + i);
	}

	nbElement += size;

	return infosSeparationBar;
}


// Compares two intervals
// according to starting times.
bool CThumbnailFolder::compareFilename(CPhotos i1, CPhotos i2)
{
	return (i1.GetPath() < i2.GetPath());
}


void CThumbnailFolder::ChangeTypeAffichage(const int& typeAffichage, bool needFindNewItem)
{
	this->needFindNewItem = needFindNewItem;

	auto _listSeparator = std::make_unique<InfosSeparationBarVector>();

	int i = 0;
	int typeLocal = typeAffichage;
	int size = CThumbnailBuffer::GetVectorSize();

	if (typeLocal == SHOW_ALL)
	{
		CTreatmentDataFolder dataYear;
		dataYear.MainTreatment(_listSeparator.get(), iconeList.get(), this, i);
	}
	else if (typeLocal == SHOW_BYYEAR)
	{
		CTreatmentDataYear dataYear;
		dataYear.MainTreatment(_listSeparator.get(), iconeList.get(), this, i);
	}
	else if (typeLocal == SHOW_BYMONTH)
	{
		CTreatmentDataMonth dataMonth;
		dataMonth.MainTreatment(_listSeparator.get(), iconeList.get(), this, i);
	}
	else if (typeLocal == SHOW_BYLOCALISATION)
	{
		CTreatmentDataLocalisation dataLocalisation;
		dataLocalisation.MainTreatment(_listSeparator.get(), iconeList.get(), this, i);
	}
	else if (typeLocal == SHOW_BYDAY)
	{
		CTreatmentDataDay dataDay;
		dataDay.MainTreatment(_listSeparator.get(), iconeList.get(), this, i);
	}


	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
		config->SetTypeAffichage(typeAffichage);
	}

	//------------------------------------------------------------------
	//Cleaning old Element : on reporte l'état sélection/affichage de
	//l'ancienne liste vers la nouvelle. La destruction des anciens éléments
	//est désormais automatique (unique_ptr) : plus de delete manuel.
	//------------------------------------------------------------------
	if (listSeparator)
	{
		for (auto& infosSeparationBar : *listSeparator)
		{
			CInfosSeparationBarExplorer* infosExplorerOld = (CInfosSeparationBarExplorer*)infosSeparationBar.get();
			for (auto& newBar : *_listSeparator)
			{
				CInfosSeparationBarExplorer* infosSeparationNew = (CInfosSeparationBarExplorer*)newBar.get();
				if (infosSeparationNew->GetLongTitle() == infosExplorerOld->GetLongTitle())
				{
					infosSeparationNew->SetSelected(infosExplorerOld->GetSelected());
					infosSeparationNew->SetShow(infosExplorerOld->GetShow());
					break;
				}
			}
		}

		// listSeparator->clear() détruirait déjà tout, mais on l'écrase carrément
		// au std::move ci-dessous, ce qui revient au même (destruction automatique
		// de l'ancien vector et de chacun de ses unique_ptr).
	}

	listSeparator = std::move(_listSeparator);

	nbElementInIconeList = iconeList->GetNbElement();

	AfterSetList();

	thumbnailPos = 0;

	threadDataProcess = true;

	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();

	needToRefresh = true;
}

void CThumbnailFolder::Init(const int& typeAffichage, const bool& isDeleteFolder, const bool& isSqlUpdate)
{
	if ((isDeleteFolder || isSqlUpdate) && nbElementInIconeList > 0)
	{
		int size = iconeList->GetNbElement();
		if (size > 0)
		{
			CIconeList * newIconeList = new CIconeList();

			GenerateList(newIconeList);

			if (newIconeList->GetNbElement() > 0)
			{
				auto old = std::move(iconeList);
				iconeList.reset(newIconeList);

				nbElementInIconeList = iconeList->GetNbElement();

				//old->EraseThumbnailListWithIcon();
			}
			// sinon newIconeList est détruit automatiquement en sortant de portée
			// (plus de delete explicite nécessaire)
		}
		else if (CThumbnailBuffer::GetVectorSize() == 0)
		{
			iconeList->EraseThumbnailListWithIcon();
		}
	}

	ChangeTypeAffichage(typeAffichage, !isDeleteFolder);
}

bool CThumbnailFolder::ItemCompFonctWithVScroll(int x, int y, CIcone* icone, CWindowMain* parent)
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

CIcone* CThumbnailFolder::FindElementWithVScroll(const int& xPos, const int& yPos)
{
	pItemCompFonct _pf = &ItemCompFonctWithVScroll;
	return iconeList->FindElementByPosition(xPos, yPos, &_pf, this);
}


CInfosSeparationBar* CThumbnailFolder::FindSeparatorElement(const int& xPos, const int& yPos)
{
	int x = xPos + posLargeur;
	int y = yPos + posHauteur;

	if (listSeparator == nullptr)
		return nullptr;

	for (auto& separatorBar : *listSeparator)
	{
		if (separatorBar != nullptr)
		{
			wxRect rc = separatorBar->GetPos();
			if ((rc.x < x && x < (rc.x + rc.width)) && (rc.y < y && y < (rc.height + rc.y)))
			{
				// Retourne un pointeur non-possédant : la propriété reste dans listSeparator.
				return separatorBar.get();
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

	for (auto& infosSeparationBar : *listSeparator)
	{
		CInfosSeparationBarExplorer* infosExplorer = (CInfosSeparationBarExplorer*)infosSeparationBar.get();
		int nbElement_localX = 0;
		int nbElement_localY = 0;

		infosSeparationBar->SetWidth(controlWidth);
		infosSeparationBar->SetWindowPos(x, y);

		y += infosSeparationBar->GetHeight();

		if (infosExplorer->GetShow())
		{
			for (auto numElement : infosSeparationBar->listElement)
			{
				CIcone* pBitmapIcone = iconeList->GetElement(numElement);
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
				CIcone* pBitmapIcone = iconeList->GetElement(numElement);
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

bool CThumbnailFolder::ItemCompFonct(int xPos, int yPos, CIcone* icone, CWindowMain* parent) /* Définit une fonction. */
{
	if (icone == nullptr || parent == nullptr)
		return false;

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

CIcone* CThumbnailFolder::FindElement(const int& xPos, const int& yPos)
{
	pItemCompFonct _pf = &ItemCompFonct;
	return iconeList->FindElementByPosition(xPos, yPos, &_pf, this);
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

		CInfosSeparationBarExplorer* infosSeparationBar = (CInfosSeparationBarExplorer*)listSeparator->at(i).get();

		if (infosSeparationBar != nullptr)
		{
			bool start = false;
			infosSeparationBar->Render(deviceContext, -posLargeur, -posHauteur);

			if (infosSeparationBar->GetShow())
			{

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

	for (auto& infosSeparationBar : *listSeparator)
	{
		CInfosSeparationBarExplorer* infosSeparationBarExplorer = (CInfosSeparationBarExplorer*)infosSeparationBar.get();
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
			auto controlSize = std::make_unique<CControlSize>();
			controlSize->controlWidth = thumbnailSizeX;
			controlSize->controlHeight = thumbnailSizeY;

			wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
			// La propriété est transférée au gestionnaire d'événement, qui est responsable
			// de la libération (pattern wx classique, SetClientData attend un pointeur nu).
			evt.SetClientData(controlSize.release());
			parent->GetEventHandler()->AddPendingEvent(evt);
		}

		if (parent != nullptr)
		{
			auto size = std::make_unique<wxSize>();
			size->x = static_cast<int>(posX);
			size->y = static_cast<int>(posY);

			wxCommandEvent evt(wxEVENT_SETPOSITION);
			// Idem : propriété transférée au gestionnaire d'événement.
			evt.SetClientData(size.release());
			parent->GetEventHandler()->AddPendingEvent(evt);
		}

		posLargeur = posX;
		posHauteur = posY;
	}

	needToRefresh = true;
}