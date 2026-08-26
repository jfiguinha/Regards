#include <header.h>
#include "ThumbnailFace.h"
#include "MainWindow.h"
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <ThumbnailDataFace.h>
#include <SqlFindFacePhoto.h>
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
	Connect(wxEVENT_SELECTALLICONE, wxCommandEventHandler(CThumbnailFace::OnSelectIcon));
}


CThumbnailFace::~CThumbnailFace(void)
{
	listSeparator.clear();
}

void CThumbnailFace::OnPictureClick(const int& numPhotoId)
{
	auto mainWindow = static_cast<CMainWindow*>(this->FindWindowById(MAINVIEWERWINDOWID));
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_ONPICTURECLICK);
		evt.SetExtraLong(numPhotoId);
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
	CIcone* icone = GetIconeById(numPhotoId);
	if (icone != nullptr)
	{
		if (icone->GetPtData() != nullptr)
		{
			CLibPicture libPicture;
			if (libPicture.TestIsVideo(icone->GetPtData()->GetFilename()))
			{
				wxWindow* window = this->FindWindowById(BITMAPWINDOWVIEWERID);
				if (window != nullptr)
				{
					wxCommandEvent evt(wxEVENT_SETPOSITION);
					evt.SetExtraLong(icone->GetPtData()->GetNumFrame());
					window->GetEventHandler()->AddPendingEvent(evt);
				}
			}
		}
	}
}


void CThumbnailFace::AddSeparatorBar(
	CIconeList* iconeListLocal,
	const wxString& libelle,
	const CFaceName& faceName,
	const std::vector<CFaceFilePath>& listPhotoFace,
	int& nbElement,
	const std::unordered_map<FaceKey, CIcone*, FaceKeyHash>& iconIndex)
{
	auto infosSeparationBar = std::make_unique<CInfosSeparationBarFace>(themeThumbnail.themeSeparation);

	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetParentWindow(this);
	infosSeparationBar->SetWidth(GetWindowWidth());
	infosSeparationBar->SetNumFace(faceName);

	const int firstElement = nbElement;

	CLibPicture libPicture;
	CSqlFacePhoto facePhoto;

	for (size_t i = 0; i < listPhotoFace.size(); ++i)
	{
		const auto& photo = listPhotoFace[i];

		const int elementIndex =
			firstElement + static_cast<int>(i);

		infosSeparationBar->listElement.push_back(elementIndex);

		FaceKey key
		{
			photo.faceFilePath,
			photo.numFace
		};

		auto it = iconIndex.find(key);

		if (it != iconIndex.end())
		{
			auto* icone = it->second;

			auto* data =
				static_cast<CThumbnailDataFace*>(icone->GetPtData());

			data->SetNumElement(elementIndex);
			icone->SetNumElement(elementIndex);

			continue;
		}

		auto* thumbnailData =
			new CThumbnailDataFace(
				photo.faceFilePath,
				photo.numFace);

		thumbnailData->SetNumPhotoId(photo.numPhoto);
		thumbnailData->SetNumElement(elementIndex);

		if (libPicture.TestIsVideo(thumbnailData->GetFilename()))
		{
			thumbnailData->SetNumFrame(
				facePhoto.GetVideoFacePosition(photo.numFace));
		}

		auto* pBitmapIcone =
			new CIcone(thumbnailData);

		pBitmapIcone->ShowSelectButton(true);
		pBitmapIcone->SetNumElement(elementIndex);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetShowDelete(true);
		pBitmapIcone->SetFilename(photo.faceFilePath);

		iconeListLocal->AddElement(pBitmapIcone);
	}

	nbElement += static_cast<int>(listPhotoFace.size());

	if (!listPhotoFace.empty())
	{
		listSeparator.push_back(std::move(infosSeparationBar));
	}
}

bool CThumbnailFace::ItemCompFonctFindFaceElement(wxString filepath, int numFace, CIcone* icone)
/* Définit une fonction. */
{
	if (icone != nullptr)
	{
		auto data = static_cast<CThumbnailDataFace*>(icone->GetPtData());
		if (data->GetFilename() == filepath && numFace == data->GetNumFace())
		{
			return true;
		}
	}
	return false;
}

CIcone* CThumbnailFace::FindFaceElement(wxString filepath, int numFace)
{
	pItemCompFonctFace _pf = &ItemCompFonctFindFaceElement;
	return iconeList->FindFaceElement(filepath, numFace, &_pf);
}

void CThumbnailFace::InitListFace()
{	
	auto viewerParam = CMainParamInit::getInstance();
	double pertinence = 0.0;
    int nbElement = 0;
	if (viewerParam != nullptr)
		pertinence = viewerParam->GetPertinenceValue();

	CIconeList* newIconeList = new CIconeList();
	CSqlFindFacePhoto sqlFindFacePhoto;
	std::vector<CFaceFilePath> listPhotoFace = sqlFindFacePhoto.GetListAllPhotoFace(pertinence);
	std::unordered_set<FaceKey, FaceKeyHash> validFaces;

	for (const auto& face : listPhotoFace)
	{
		validFaces.insert(
			{
				face.faceFilePath,
				face.numFace
			});
	}

	for (int i = 0; i < iconeList->GetNbElement(); ++i)
	{
		auto* icone = iconeList->GetElement(i);

		auto* data =
			static_cast<CThumbnailDataFace*>(icone->GetPtData());

		FaceKey key
		{
			data->GetFilename(),
			data->GetNumFace()
		};

		if (validFaces.find(key) != validFaces.end())
		{
			newIconeList->AddElement(icone);
		}
		else
		{
			iconeList->RemoveElement(i);
			--i;
		}
	}
}

void CThumbnailFace::init()
{
	auto viewerParam = CMainParamInit::getInstance();
	threadDataProcess = false;
	double pertinence = 0.0;
	if (viewerParam != nullptr)
		pertinence = viewerParam->GetPertinenceValue();


	InitListFace();

	std::unordered_map<FaceKey, CIcone*, FaceKeyHash> iconIndex;

	for (int i = 0; i < iconeList->GetNbElement(); ++i)
	{
		auto* icone = iconeList->GetElement(i);

		if (auto* data =
			static_cast<CThumbnailDataFace*>(icone->GetPtData()))
		{
			iconIndex.insert(
				{
					{ data->GetFilename(), data->GetNumFace() },
					icone
				});
		}
	}


	listSeparator.clear();

	nbElement = 0;

	CSqlFindFacePhoto sqlFindFacePhoto;
	std::vector<CFaceName> listFace = sqlFindFacePhoto.GetListFaceName();
	for (int i = 0; i < listFace.size(); i++)
	{
		std::vector<CFaceFilePath> listPhotoFace = sqlFindFacePhoto.GetListPhotoFace(listFace.at(i).numFace, pertinence);
		AddSeparatorBar(iconeList.get(), listFace.at(i).faceName, listFace.at(i), listPhotoFace, nbElement, iconIndex);
	}


	nbElementInIconeList = iconeList->GetNbElement();

	AfterSetList();

	thumbnailPos = 0;

	threadDataProcess = true;

	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();

	needToRefresh = true;
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
	return iconeList->FindElementByPosition(xPos, yPos, &_pf, this);
}




void CThumbnailFace::MoveIcone(const int& numElement, const int& numFace)
{
	for (auto& separatorBar : listSeparator)
	{
		auto infosSeparationBarFace = static_cast<CInfosSeparationBarFace*>(separatorBar.get());
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

	auto it = listSeparator.begin();

	while (it != listSeparator.end())
	{
		auto* faceBar = static_cast<CInfosSeparationBarFace*>(it->get());

		if (faceBar != nullptr &&
			faceBar->listElement.empty())
		{
			sqlfaceLabel.DeleteFaceLabelDatabase(
				faceBar->GetNumFace());

			it = listSeparator.erase(it);
		}
		else
		{
			++it;
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
	CSqlFaceRecognition faceRecognition;
	CSqlFaceLabel sqlfaceLabel;
	int numFace = sqlfaceLabel.GetNumFace(faceName);

	for (auto& separatorBar : listSeparator)
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
						auto thumbnailData = static_cast<CThumbnailDataFace*>(icone->GetPtData());
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
	for (auto& separatorBar : listSeparator)
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
						auto thumbnailData = static_cast<CThumbnailDataFace*>(icone->GetPtData());
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

	for (auto& separatorBar : listSeparator)
	{
		if (separatorBar != nullptr)
		{
			wxRect rc = separatorBar->GetPos();
			if ((rc.x < x && x < (rc.x + rc.width)) && (rc.y < y && y < (rc.height + rc.y)))
			{
				return separatorBar.get();
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
			auto faceSeparator = static_cast<CInfosSeparationBarFace*>(listSeparator[i].get()); // static_cast<CInfosSeparationBarFace*>(listSeparator[i]);
			numFace = faceSeparator->GetNumFace();
		}
		else
		{
			auto separatorBarFirst = static_cast<CInfosSeparationBarFace*>(listSeparator[i].get());
			auto separatorBarSecond = static_cast<CInfosSeparationBarFace*>(listSeparator[i + 1].get());
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
		for (auto& separatorBar : listSeparator)
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
							auto thumbnailData = static_cast<CThumbnailDataFace*>(icone->GetPtData());
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
			bool select = faceSeparator->GetSelected();
			faceSeparator->OnClick(x, y);
			if (select != faceSeparator->GetSelected())
			{
				auto eventChange = new wxCommandEvent(wxEVENT_SELECTALLICONE);
				eventChange->SetClientData(faceSeparator);
				wxQueueEvent(this, eventChange);
			}
			mouseClickBlock = false;
		}
	}
}

void CThumbnailFace::OnSelectIcon(wxCommandEvent& event)
{

	auto faceSeparator = static_cast<CInfosSeparationBarFace*>(event.GetClientData());
	if (faceSeparator != nullptr)
	{
		for (auto numElement : faceSeparator->listElement)
		{
			CIcone* icone = iconeList->GetElement(numElement);
			if (icone != nullptr)
			{
				if (faceSeparator->GetSelected())
					icone->SetChecked(true);
				else
					icone->SetChecked(false);
			}
		}
	}
}



void CThumbnailFace::DeleteIcone(CIcone* numSelect)
{
	auto face_thumbnail = static_cast<CThumbnailDataFace*>(numSelect->GetPtData());
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
	return iconeList->FindElementByPosition(xPos, yPos, &_pf, this);
}
