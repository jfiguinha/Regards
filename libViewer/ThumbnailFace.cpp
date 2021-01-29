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

CThumbnailFace::CThumbnailFace(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity)
{
	
	barseparationHeight = 40;
	widthThumbnail = 0;
	heightThumbnail = 0;
	flipHorizontal = false;
	flipVertical = true;
}


CThumbnailFace::~CThumbnailFace(void)
{
	for (CInfosSeparationBar* infosSeparationBar : listSeparator)
	{
		delete(infosSeparationBar);
	}

	listSeparator.clear();
}

void CThumbnailFace::OnPictureClick(CThumbnailData * data)
{
    CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_ONPICTURECLICK);
		evt.SetExtraLong(data->GetNumPhotoId());
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}

	CLibPicture libPicture;
	if (libPicture.TestIsVideo(data->GetFilename()))
	{
		wxWindow * window = this->FindWindowById(SHOWVIDEOVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_SETPOSITION);
			evt.SetExtraLong(data->GetNumFrame());
			window->GetEventHandler()->AddPendingEvent(evt);
		}
	}
}


void CThumbnailFace::AddSeparatorBar(const wxString &libelle, const CFaceName & faceName, const std::vector<CFaceFilePath> & listPhotoFace, int &nbElement)
{
	CInfosSeparationBarFace * infosSeparationBar = new CInfosSeparationBarFace(themeThumbnail.themeSeparation);
	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetParentWindow(this);
	infosSeparationBar->SetWidth(GetWindowWidth());
	infosSeparationBar->SetNumFace(faceName);

	for (auto i = 0; i < listPhotoFace.size();i++)
	{
		CFaceFilePath numFace = listPhotoFace.at(i);
		infosSeparationBar->listElement.push_back((int)iconeList->GetNbElement());

		CSqlFaceThumbnail * thumbnailData = new CSqlFaceThumbnail(numFace.faceFilePath, numFace.numFace);
		thumbnailData->SetNumPhotoId(numFace.numPhoto);
		thumbnailData->SetNumElement(nbElement++);

		CLibPicture libPicture;
		if (libPicture.TestIsVideo(thumbnailData->GetFilename()))
		{
			CSqlFacePhoto facePhoto;
			int positionVideo = facePhoto.GetVideoFacePosition(numFace.numFace);
			thumbnailData->SetNumFrame(positionVideo);
		}


		CIcone * pBitmapIcone = new CIcone();
		pBitmapIcone->ShowSelectButton(true);
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		iconeList->AddElement(pBitmapIcone);
	}

	if (listPhotoFace.size() > 0)
		listSeparator.push_back(infosSeparationBar);
}

void CThumbnailFace::SortSeparator(CInfosSeparationBar * infosSeparationBar)
{
	for(int i = 0;i < infosSeparationBar->listElement.size() - 1;i++)
	{
		int min = i;
		int numElement = infosSeparationBar->listElement[i];
		CIcone * pBitmapIcone = iconeList->GetElement(numElement);
        if(pBitmapIcone != nullptr)
        {
            CSqlFaceThumbnail * thumbnailData = (CSqlFaceThumbnail *)pBitmapIcone->GetData();
            if(thumbnailData != nullptr)
            {
                wxString filename = thumbnailData->GetFilename();
                wxString filenameToChange = filename;
                for(int j = i + 1;j < infosSeparationBar->listElement.size();j++)
                {
                    int numElement2 = infosSeparationBar->listElement[j];
                    CIcone * pBitmapIcone2 = iconeList->GetElement(numElement2);
                    if(pBitmapIcone2 != nullptr)
                    {
                        CSqlFaceThumbnail * thumbnailData2 = (CSqlFaceThumbnail *)pBitmapIcone2->GetData();
                        if(thumbnailData2 != nullptr)
                        {
                            wxString filename2 = thumbnailData2->GetFilename();
                            if(filenameToChange > filename2)
                            {
                                filenameToChange = filename2;
                                min = j;
                            }                              
                        }
                    }
                }
            }
           
        }


        if (min != i)
        {
			//int element = infosSeparationBar->listElement[min];
            //std::swap(tab[i], tab[min]); // defined in <algorithm> before C++11
			//infosSeparationBar->listElement[min] = infosSeparationBar->listElement[i];
			//infosSeparationBar->listElement[i] = element;

			int element = infosSeparationBar->listElement[i];
			infosSeparationBar->listElement[i] = infosSeparationBar->listElement[min];
			infosSeparationBar->listElement[min] = element;
        }
	}
}


void CThumbnailFace::Init()
{
	CMainParam * viewerParam = (CMainParam *)CMainParamInit::getInstance();
	threadDataProcess = false;

	double pertinence = 0.0;
	if(viewerParam != nullptr)
		pertinence = viewerParam->GetPertinenceValue();


	//---------------------------------
	//Sauvegarde de l'état
	//---------------------------------

	EraseThumbnailList();

	for (CInfosSeparationBar * infosSeparationBar : listSeparator)
	{
		delete(infosSeparationBar);
	}

	listSeparator.clear();

	nbElement = 0;

	CSqlFindFacePhoto sqlFindFacePhoto;
	std::vector<CFaceName> listFace = sqlFindFacePhoto.GetListFaceName();
	for(int i = 0;i < listFace.size();i++)
	{
		std::vector<CFaceFilePath> listPhotoFace = sqlFindFacePhoto.GetListPhotoFace(listFace.at(i).numFace, pertinence);
		AddSeparatorBar(listFace.at(i).faceName, listFace.at(i), listPhotoFace, nbElement);
	}

	SetNbFiles(nbElement);

	AfterSetList();

	thumbnailPos = 0;

	threadDataProcess = true;

	for(int k = 0;k < listSeparator.size();k++)
	{
		CInfosSeparationBar * infosSeparationBar = listSeparator.at(k);
		SortSeparator(infosSeparationBar);
	}

	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();

	Refresh();
}


CIcone * CThumbnailFace::FindIcone(const int &photoId)
{
	if(!threadDataProcess)
		return nullptr;

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

bool CThumbnailFace::ItemCompFonctWithVScroll(int x, int y, CIcone * icone, CWindowMain * parent)   /* Définit une fonction. */
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

CIcone * CThumbnailFace::FindElementWithVScroll(const int &xPos, const int &yPos)
{
	pItemCompFonct _pf = &ItemCompFonctWithVScroll;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
	/*
	int x = xPos;
	int y = yPos;
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

	return nullptr;
	*/
}


void CThumbnailFace::MoveIcone(const int &numElement, const int &numFace)
{
	for (CInfosSeparationBar * separatorBar : listSeparator)
	{
		CInfosSeparationBarFace * infosSeparationBarFace = (CInfosSeparationBarFace *)separatorBar;
		if(numFace == infosSeparationBarFace->GetNumFace())
		{
			infosSeparationBarFace->listElement.push_back(numElement);
			return;
		}
	}
}

//-----------------------------------------------------------------
//
//-----------------------------------------------------------------
void CThumbnailFace::MoveFace(const wxString &faceName)
{
	vector<CThumbnailData *> listItem;
	GetSelectItem(listItem);

	CSqlFaceRecognition faceRecognition;
	CSqlFaceLabel sqlfaceLabel;
	int numFace = sqlfaceLabel.GetNumFace(faceName);

	for (CInfosSeparationBar * separatorBar : listSeparator)
	{
		if (separatorBar != nullptr)
		{
			vector<int> numElementToDelete;
			for(int i = 0;i < separatorBar->listElement.size();i++)
			{
				int numElement = separatorBar->listElement.at(i);
				CIcone * icone = iconeList->GetElement(numElement);
                if(icone != nullptr)
                {
                    if(icone->IsChecked())
                    {
                        CSqlFaceThumbnail * thumbnailData = (CSqlFaceThumbnail *)icone->GetData();
                        int numFaceCompatible = faceRecognition.GetCompatibleFace(thumbnailData->GetNumFace());
                        if(numFaceCompatible != numFace)
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

	Refresh();
}

void CThumbnailFace::DeleteEmptyFace()
{
	CSqlFaceLabel sqlfaceLabel;
	//Test Si Infos Bar Vide
	for(int i = 0;i < listSeparator.size();i++)
	{
		CInfosSeparationBarFace * infosSeparationBarFace = (CInfosSeparationBarFace *)listSeparator.at(i);
		if(infosSeparationBarFace != nullptr)
		{
			if(infosSeparationBarFace->listElement.size() == 0)
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
CInfosSeparationBar * CThumbnailFace::FindSeparatorElement(const int &xPos, const int &yPos)
{

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


void CThumbnailFace::FindOtherElement(wxDC * dc, const int &x, const int &y)
{
	CInfosSeparationBar * separator = FindSeparatorElement(x, y);
	if (separator != nullptr)
	{
		CInfosSeparationBarFace * faceSeparator = (CInfosSeparationBarFace *)separator;
		if (faceSeparator != nullptr)
		{
			faceSeparator->OnClick(x, y);

            for (auto numElement : separator->listElement)	
			{
				CIcone * icone = iconeList->GetElement(numElement);
                if(icone != nullptr)
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

	/*
    if (!noVscroll)
        ResizeThumbnailWithVScroll();
        
    widthThumbnail = GetWindowWidth();
    heightThumbnail = GetWindowHeight();
       
    UpdateScroll();
	*/
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
		if ((elementByRow * themeThumbnail.themeIcone.GetWidth()) < (GetWindowWidth() + positionXOld))
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

}

bool CThumbnailFace::ItemCompFonct(int xPos, int yPos, CIcone * icone, CWindowMain * parent)   /* Définit une fonction. */
{
	if (icone != nullptr && parent != nullptr)
	{
		CThumbnailFace * face = (CThumbnailFace *)parent;
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

CIcone * CThumbnailFace::FindElement(const int &xPos, const int &yPos)
{

	if(!threadDataProcess)
		return nullptr;

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


void CThumbnailFace::RenderIconeWithVScroll(wxDC * deviceContext)
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

void CThumbnailFace::UpdateScrollWithVScroll()
{
    printf("CThumbnailFace::UpdateScrollWithVScroll \n");
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

            
    printf("CThumbnailFace::UpdateScrollWithVScroll old %d %d \n", oldthumbnailSizeX, oldthumbnailSizeY);
    printf("CThumbnailFace::UpdateScrollWithVScroll new %d %d \n", thumbnailSizeX, thumbnailSizeY);


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
#endif