#include <header.h>
#include "ThumbnailFace.h"
#include "MainWindow.h"
#include "ViewerParamInit.h"
#include "ViewerParam.h"
#include <window_id.h>
#include <LibResource.h>
#include <SqlFaceThumbnail.h>
#include <SqlFindFacePhoto.h>
#include <ConvertUtility.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarVerticalWnd.h>
#include <ScrollbarWnd.h>
#include <SqlPhotos.h>
#include <SqlFaceRecognition.h>
#include <SqlFaceLabel.h>
using namespace Regards::Viewer;
using namespace Regards::Sqlite;


CThumbnailFace::CThumbnailFace(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity)
{
	
	barseparationHeight = 40;
	widthThumbnail = 0;
	heightThumbnail = 0;
}


CThumbnailFace::~CThumbnailFace(void)
{
	for (CInfosSeparationBar * infosSeparationBar : listSeparator)
	{
		delete(infosSeparationBar);
	}

	listSeparator.clear();
}

void CThumbnailFace::SetActifItem(const int &numItem, const bool &move)
{
    if (numItem >= iconeList->GetNbElement())
        return;
    
    if (numSelect != NULL)
        numSelect->SetSelected(false);
    
    if (numActif != nullptr)
    {
        numActif->SetActive(false);
        //numActif->RenderIcone(&winDC);
    }
    
    numActif = iconeList->GetElement(numItem);

    if (move)
    {
        CScrollbarVerticalWnd * scrollV = scrollbar->GetVScrollbar();
        CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();
        
        if (numItem == 0)
        {
            scrollV->SetPosition(0);
            scrollH->SetPosition(0);
        }
        else{
            if (!scrollV->IsMoving())
            {
                wxRect rect = numActif->GetPos();
               
                //Positionnement au milieu

                int yPos = max((rect.y - scrollV->GetScreenHeight() / 2),0);
                int xPos = max((rect.x - scrollH->GetScreenWidth() / 2), 0);

                scrollV->SetPosition(yPos);
                scrollH->SetPosition(xPos);
                
                
                if (GetWindowWidth() < GetWidth())
                    posLargeur = scrollH->GetPosition();
                
                if (GetWindowHeight() < GetHeight())
                    posHauteur = scrollV->GetPosition();
                
            }
        }
    }
    
    numSelect = iconeList->GetElement(numItem);
    
    if (numSelect != nullptr)
    {
        numSelect->SetSelected(true);
        //numSelect->RenderIcone(&winDC);
    }

	this->Refresh();
}

void CThumbnailFace::OnPictureClick(CThumbnailData * data)
{
    CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxString * photoPath = new wxString();
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_FACECLICK);
		*photoPath = data->GetFilename();
		evt.SetClientData(photoPath);
		evt.SetExtraLong(data->GetNumPhotoId());
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
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

		CIcone * pBitmapIcone = new CIcone(nullptr);
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
	CViewerParam * viewerParam = (CViewerParam *)CViewerParamInit::getInstance();
	threadDataProcess = false;

	double pertinence = 0.0;
	if(viewerParam != nullptr)
		pertinence = viewerParam->GetPertinenceValue();


	//---------------------------------
	//Sauvegarde de l'état
	//---------------------------------

	int numPhotoIdActif = 0;
	int numPhotoIdSelect = 0;

	if (numSelect != nullptr)
	{
		CThumbnailData * data = numSelect->GetData();
		if (data != nullptr)
			numPhotoIdSelect = data->GetNumPhotoId();
	}

	if (numActif != nullptr)
	{
		CThumbnailData * data = numActif->GetData();
		if (data != nullptr)
			numPhotoIdActif = data->GetNumPhotoId();
	}


	//InitScrollingPos();

	EraseThumbnailList();

	numSelect = nullptr;
	numActif = nullptr;

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

	if (numPhotoIdSelect > 0)
	{
		CIcone * icone = FindIcone(numPhotoIdSelect);
		if (icone != nullptr)
		{
			icone->SetSelected(true);
		}
	}

	if (numPhotoIdActif > 0)
	{
		CIcone * icone = FindIcone(numPhotoIdActif);
		if (icone != nullptr)
		{
			icone->SetActive(true);
		}
	}

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



CIcone * CThumbnailFace::FindElementWithVScroll(const int &xPos, const int &yPos)
{
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
    if (!noVscroll)
        ResizeThumbnailWithVScroll();
        
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

CIcone * CThumbnailFace::FindElement(const int &xPos, const int &yPos)
{

	if(!threadDataProcess)
		return nullptr;

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
}


void CThumbnailFace::RenderIconeWithVScroll(wxDC * deviceContext)
{
    
	posHauteur = scrollbar->GetPosHauteur();
	posLargeur = scrollbar->GetPosLargeur();


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


            scrollbar->SetControlSize(thumbnailSizeX, thumbnailSizeY);
            scrollbar->SetPosition(posX, posY);
            posLargeur = scrollbar->GetPosLargeur();
            posHauteur = scrollbar->GetPosHauteur();
        }

}
