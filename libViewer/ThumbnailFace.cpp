#include <header.h>
#include "ThumbnailFace.h"
#include "MainWindow.h"
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <SqlFaceThumbnail.h>
#include <SqlFindFacePhoto.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarWnd.h>
#include <InfosSeparationBarFace.h>
#include <SqlFaceRecognition.h>
#include <SqlFaceLabel.h>
#include <libPicture.h>
#include <SqlFacePhoto.h>
using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::Picture;

CThumbnailFace::CThumbnailFace(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
                               const bool& testValidity)
	: CThumbnailVerticalSeparator(parent, id, themeThumbnail, testValidity)
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
		wxWindow* window = this->FindWindowById(BITMAPWINDOWVIEWERID);
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


		auto pBitmapIcone = std::shared_ptr<CIcone>(new CIcone());
		pBitmapIcone->ShowSelectButton(true);
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetShowDelete(true);
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
		std::vector<CFaceFilePath> listPhotoFace = sqlFindFacePhoto.GetListPhotoFace(listFace.at(i).numFace, pertinence);
		AddSeparatorBar(iconeListLocal, listFace.at(i).faceName, listFace.at(i), listPhotoFace, nbElement);
	}

	int size = iconeListLocal->GetNbElement();

#ifndef USE_TBB_VECTOR
	for (auto i = 0; i < size; i++)
#else
	tbb::parallel_for(0, size, 1, [=](int i)
#endif
	{
		int photo = iconeListLocal->GetPhotoId(i);
		std::shared_ptr<CIcone> ico = iconeList->FindElementPhotoId(photo);
		if (ico != nullptr)
		{
			std::shared_ptr<CIcone> iconew = iconeListLocal->GetElement(i);
			if (iconew != nullptr)
			{
				iconew->SetChecked(ico->IsChecked());
				iconew->SetState(ico->GetState());
			}

		}
	}
#ifdef USE_TBB_VECTOR  
	);
#endif


	oldIconeList = iconeList;
	iconeList = iconeListLocal;

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

bool CThumbnailFace::ItemCompFonctWithVScroll(int x, int y, std::shared_ptr<CIcone> icone, CWindowMain* parent)
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

std::shared_ptr<CIcone> CThumbnailFace::FindElementWithVScroll(const int& xPos, const int& yPos)
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

	CSqlFacePhoto facePhoto;
	facePhoto.RebuildLink();
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
				std::shared_ptr<CIcone> icone = iconeList->GetElement(numElement);
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

	needToRefresh = true;
}

vector<int> CThumbnailFace::GetFaceSelectID()
{
	vector<int> listFace;
	for (CInfosSeparationBar* separatorBar : listSeparator)
	{
		if (separatorBar != nullptr)
		{
			vector<int> numElementToDelete;
			for (int i = 0; i < separatorBar->listElement.size(); i++)
			{
				int numElement = separatorBar->listElement.at(i);
				std::shared_ptr<CIcone> icone = iconeList->GetElement(numElement);
				if (icone != nullptr)
				{
					bool needToMove = false;
					if (icone->IsChecked())
					{
						auto thumbnailData = static_cast<CSqlFaceThumbnail*>(icone->GetData());
						listFace.push_back(thumbnailData->GetNumFace());
					}
				}
			}
		}
	}
	return listFace;
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
					std::shared_ptr<CIcone> icone = iconeList->GetElement(numElement);
					if (icone != nullptr)
					{
						bool needToMove = false;
						if (icone->IsChecked())
						{
							auto thumbnailData = static_cast<CSqlFaceThumbnail*>(icone->GetData());
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

		wxWindow* mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
		auto eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
		wxQueueEvent(mainWnd, eventChange);
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
				std::shared_ptr<CIcone> icone = iconeList->GetElement(numElement);
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

void CThumbnailFace::DeleteIcone(std::shared_ptr<CIcone> numSelect)
{
	auto face_thumbnail = static_cast<CSqlFaceThumbnail*>(numSelect->GetData());
	if (face_thumbnail != nullptr)
	{
		CSqlFacePhoto facePhoto;
		facePhoto.DeleteNumFace(face_thumbnail->GetNumFace());

		DeleteEmptyFace();

		wxWindow* mainWnd = this->FindWindowById(MAINVIEWERWINDOWID);
		auto eventChange = new wxCommandEvent(wxEVT_CRITERIACHANGE);
		wxQueueEvent(mainWnd, eventChange);

		init();
	}
}

bool CThumbnailFace::ItemCompFonct(int xPos, int yPos, std::shared_ptr<CIcone> icone, CWindowMain* parent) /* Définit une fonction. */
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

std::shared_ptr<CIcone> CThumbnailFace::FindElement(const int& xPos, const int& yPos)
{
	if (!threadDataProcess)
		return nullptr;

	pItemCompFonct _pf = &ItemCompFonct;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
}

