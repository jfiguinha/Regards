#include <header.h>
#ifndef __NOFACE_DETECTION__
#include "ThumbnailFace.h"
#include "MainWindow.h"
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <SqlFaceThumbnail.h>
#include <SqlFindFacePhoto.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarWnd.h>
#include <SqlFaceRecognition.h>
#include <SqlFaceLabel.h>
#include <libPicture.h>
#include <SqlFacePhoto.h>
using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::Picture;

CThumbnailFace::CThumbnailFace(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
                               const bool& testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity)
{
	barseparationHeight = 40;
	widthThumbnail = 0;
	heightThumbnail = 0;
	flipHorizontal = false;
	flipVertical = true;
	enableDragAndDrop = true;
	moveOnPaint = false;
}


CThumbnailFace::~CThumbnailFace(void)
{
	for (CInfosSeparationBar* infosSeparationBar : listSeparator)
	{
		delete(infosSeparationBar);
	}

	listSeparator.clear();
}

void CThumbnailFace::OnPictureClick(CThumbnailData* data)
{
	auto mainWindow = static_cast<CMainWindow*>(this->FindWindowById(MAINVIEWERWINDOWID));
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_ONPICTURECLICK);
		evt.SetExtraLong(data->GetNumPhotoId());
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}

	CLibPicture libPicture;
	if (libPicture.TestIsVideo(data->GetFilename()))
	{
		wxWindow* window = this->FindWindowById(SHOWVIDEOVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_SETPOSITION);
			evt.SetExtraLong(data->GetNumFrame());
			window->GetEventHandler()->AddPendingEvent(evt);
		}
	}
}


void CThumbnailFace::AddSeparatorBar(CIconeList* iconeListLocal, const wxString& libelle, const CFaceName& faceName,
                                     const std::vector<CFaceFilePath>& listPhotoFace, int& nbElement)
{
	auto infosSeparationBar = new CInfosSeparationBarFace(themeThumbnail.themeSeparation);
	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetParentWindow(this);
	infosSeparationBar->SetWidth(GetWindowWidth());
	infosSeparationBar->SetNumFace(faceName);

	for (auto i = 0; i < listPhotoFace.size(); i++)
	{
		CFaceFilePath numFace = listPhotoFace.at(i);
		infosSeparationBar->listElement.push_back(iconeListLocal->GetNbElement());

		auto thumbnailData = new CSqlFaceThumbnail(numFace.faceFilePath, numFace.numFace);
		thumbnailData->SetNumPhotoId(numFace.numPhoto);
		thumbnailData->SetNumElement(nbElement++);

		CLibPicture libPicture;
		if (libPicture.TestIsVideo(thumbnailData->GetFilename()))
		{
			CSqlFacePhoto facePhoto;
			int positionVideo = facePhoto.GetVideoFacePosition(numFace.numFace);
			thumbnailData->SetNumFrame(positionVideo);
		}


		auto pBitmapIcone = new CIcone();
		pBitmapIcone->ShowSelectButton(true);
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		iconeListLocal->AddElement(pBitmapIcone);
	}

	if (listPhotoFace.size() > 0)
		listSeparator.push_back(infosSeparationBar);
}

void CThumbnailFace::init()
{
	auto iconeListLocal = new CIconeList();
	CIconeList* oldIconeList = nullptr;
	auto viewerParam = CMainParamInit::getInstance();
	threadDataProcess = false;
	double pertinence = 0.0;
	if (viewerParam != nullptr)
		pertinence = viewerParam->GetPertinenceValue();


	//---------------------------------
	//Sauvegarde de l'état
	//---------------------------------
	for (CInfosSeparationBar* infosSeparationBar : listSeparator)
	{
		delete(infosSeparationBar);
	}

	listSeparator.clear();

	nbElement = 0;

	CSqlFindFacePhoto sqlFindFacePhoto;
	std::vector<CFaceName> listFace = sqlFindFacePhoto.GetListFaceName();
	for (int i = 0; i < listFace.size(); i++)
	{
		std::vector<CFaceFilePath> listPhotoFace = sqlFindFacePhoto.
			GetListPhotoFace(listFace.at(i).numFace, pertinence);
		AddSeparatorBar(iconeListLocal, listFace.at(i).faceName, listFace.at(i), listPhotoFace, nbElement);
	}


	lockIconeList.lock();
	oldIconeList = iconeList;
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

	Refresh();
}

bool CThumbnailFace::ItemCompFonctWithVScroll(int x, int y, CIcone* icone, CWindowMain* parent)
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

CIcone* CThumbnailFace::FindElementWithVScroll(const int& xPos, const int& yPos)
{
	pItemCompFonct _pf = &ItemCompFonctWithVScroll;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
}


void CThumbnailFace::MoveIcone(const int& numElement, const int& numFace)
{
	for (CInfosSeparationBar* separatorBar : listSeparator)
	{
		auto infosSeparationBarFace = static_cast<CInfosSeparationBarFace*>(separatorBar);
		if (numFace == infosSeparationBarFace->GetNumFace())
		{
			infosSeparationBarFace->listElement.push_back(numElement);
			return;
		}
	}
}

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
void CThumbnailFace::MoveFace(const wxString& faceName)
{
	vector<CThumbnailData*> listItem;
	GetSelectItem(listItem);

	CSqlFaceRecognition faceRecognition;
	CSqlFaceLabel sqlfaceLabel;
	int numFace = sqlfaceLabel.GetNumFace(faceName);

	for (CInfosSeparationBar* separatorBar : listSeparator)
	{
		if (separatorBar != nullptr)
		{
			vector<int> numElementToDelete;
			for (int i = 0; i < separatorBar->listElement.size(); i++)
			{
				int numElement = separatorBar->listElement.at(i);
				CIcone* icone = iconeList->GetElement(numElement);
				if (icone != nullptr)
				{
					if (icone->IsChecked())
					{
						auto thumbnailData = static_cast<CSqlFaceThumbnail*>(icone->GetData());
						int numFaceCompatible = faceRecognition.GetCompatibleFace(thumbnailData->GetNumFace());
						if (numFaceCompatible != numFace)
						{
							MoveIcone(numElement, numFace);
							faceRecognition.MoveFaceRecognition(thumbnailData->GetNumFace(), numFace);
							separatorBar->listElement.erase(separatorBar->listElement.begin() + i);
							i--;
						}
					}
					icone->SetChecked(false);
				}
			}
		}
	}

	DeleteEmptyFace();


	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();

	wxWindow* mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
	auto eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
	wxQueueEvent(mainWnd, eventChange);

	Refresh();
}

void CThumbnailFace::DeleteEmptyFace()
{
	CSqlFaceLabel sqlfaceLabel;
	//Test Si Infos Bar Vide
	for (int i = 0; i < listSeparator.size(); i++)
	{
		auto infosSeparationBarFace = static_cast<CInfosSeparationBarFace*>(listSeparator.at(i));
		if (infosSeparationBarFace != nullptr)
		{
			if (infosSeparationBarFace->listElement.size() == 0)
			{
				sqlfaceLabel.DeleteFaceLabelDatabase(infosSeparationBarFace->GetNumFace());

				listSeparator.erase(listSeparator.begin() + i);
				delete infosSeparationBarFace;
				infosSeparationBarFace = nullptr;
				i--;
			}
		}
	}
}


//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
CInfosSeparationBar* CThumbnailFace::FindSeparatorElement(const int& xPos, const int& yPos)
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


int CThumbnailFace::FindSeparatorFace(const int& xPos, const int& yPos)
{
	//int x = xPos + posLargeur;
	int y = yPos + posHauteur;
	int numFace = 0;
	for (int i = 0; i < listSeparator.size(); i++)
	{
		if (i == listSeparator.size() - 1)
		{
			auto faceSeparator = static_cast<CInfosSeparationBarFace*>(listSeparator[i]);
			numFace = faceSeparator->GetNumFace();
		}
		else
		{
			auto separatorBarFirst = static_cast<CInfosSeparationBarFace*>(listSeparator[i]);
			auto separatorBarSecond = static_cast<CInfosSeparationBarFace*>(listSeparator[i + 1]);
			wxRect rcFirst = separatorBarFirst->GetPos();
			wxRect rcSecond = separatorBarSecond->GetPos();
			if (y > (rcFirst.y + rcFirst.height) && y < rcSecond.y)
			{
				numFace = separatorBarFirst->GetNumFace();
				break;
			}
		}
	}
	return numFace;
}

void CThumbnailFace::OnMouseRelease(const int& x, const int& y)
{
	bool faceMove = false;
	int numFace = FindSeparatorFace(x, y);
	CSqlFaceRecognition faceRecognition;

	if (numFace != 0)
	{
		for (CInfosSeparationBar* separatorBar : listSeparator)
		{
			if (separatorBar != nullptr)
			{
				vector<int> numElementToDelete;
				for (int i = 0; i < separatorBar->listElement.size(); i++)
				{
					int numElement = separatorBar->listElement.at(i);
					CIcone* icone = iconeList->GetElement(numElement);
					if (icone != nullptr)
					{
						bool needToMove = false;
						if (icone->IsChecked())
						{
							auto thumbnailData = static_cast<CSqlFaceThumbnail*>(icone->GetData());
							if (thumbnailData->GetNumFace() != numFace)
							{
								int numFaceCompatible = faceRecognition.GetCompatibleFace(thumbnailData->GetNumFace());
								if (numFaceCompatible != numFace)
								{
									faceMove = true;
									needToMove = true;
									MoveIcone(numElement, numFace);
									faceRecognition.MoveFaceRecognition(thumbnailData->GetNumFace(), numFace);
									separatorBar->listElement.erase(separatorBar->listElement.begin() + i);
									i--;
								}
							}
						}
						if (needToMove)
							icone->SetChecked(false);
					}
				}
			}
		}
	}

	if (faceMove)
	{
		DeleteEmptyFace();
		widthThumbnail = 0;
		heightThumbnail = 0;
		ResizeThumbnail();
	}
}

void CThumbnailFace::FindOtherElement(wxDC* dc, const int& x, const int& y)
{
	CInfosSeparationBar* separator = FindSeparatorElement(x, y);
	if (separator != nullptr)
	{
		auto faceSeparator = static_cast<CInfosSeparationBarFace*>(separator);
		if (faceSeparator != nullptr)
		{
			faceSeparator->OnClick(x, y);

			for (auto numElement : separator->listElement)
			{
				CIcone* icone = iconeList->GetElement(numElement);
				if (icone != nullptr)
				{
					if (faceSeparator->GetSelected())
						icone->SetChecked(true);
					else
						icone->SetChecked(false);
				}

				//icone->RenderIcone(dc);
			}
		}
	}
}

void CThumbnailFace::ResizeThumbnail()
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

void CThumbnailFace::ResizeThumbnailWithVScroll()
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

bool CThumbnailFace::ItemCompFonct(int xPos, int yPos, CIcone* icone, CWindowMain* parent) /* Définit une fonction. */
{
	if (icone != nullptr && parent != nullptr)
	{
		auto face = static_cast<CThumbnailFace*>(parent);
		wxRect rc = icone->GetPos();
		int left = rc.x - face->posLargeur;
		int right = rc.x + rc.width - face->posLargeur;
		int top = rc.y - face->posHauteur;
		int bottom = rc.y + rc.height - face->posHauteur;
		if ((left < xPos && xPos < right) && (top < yPos && yPos < bottom))
		{
			return true;
		}
	}
	return false;
}

CIcone* CThumbnailFace::FindElement(const int& xPos, const int& yPos)
{
	if (!threadDataProcess)
		return nullptr;

	pItemCompFonct _pf = &ItemCompFonct;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
}


void CThumbnailFace::RenderIconeWithVScroll(wxDC* deviceContext)
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

void CThumbnailFace::UpdateScrollWithVScroll()
{
	printf("CThumbnailFace::UpdateScrollWithVScroll \n");
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


	printf("CThumbnailFace::UpdateScrollWithVScroll old %d %d \n", oldthumbnailSizeX, oldthumbnailSizeY);
	printf("CThumbnailFace::UpdateScrollWithVScroll new %d %d \n", thumbnailSizeX, thumbnailSizeY);


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
	this->Refresh();
}
#endif
