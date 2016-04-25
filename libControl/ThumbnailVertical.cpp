#include "ThumbnailVertical.h"
#include <ThumbnailDataSQL.h>
#include <Photos.h>
//#include <omp.h>
#include <FileUtility.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include "ScrollbarWnd.h"
using namespace Regards::Control;

CThumbnailVertical::CThumbnailVertical(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail)
	: CThumbnail(parent, id, statusBarInterface, themeThumbnail)
{
	typeAffichage = TYPEPHOTO;
	numCatalog = 0;
	numCategorie = 0;
	key = L"";
}


CThumbnailVertical::~CThumbnailVertical(void)
{

}

int CThumbnailVertical::GetTypeAffichage()
{
	return typeAffichage;
}

int CThumbnailVertical::GetNumCatalog()
{
	return numCatalog;
}

int CThumbnailVertical::GetNumCategorie()
{
	return numCategorie;
}

wxString CThumbnailVertical::GetKey()
{
	return key;
}

CIcone * CThumbnailVertical::FindElement(const int &xPos, const int &yPos)
{
	CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();
	CScrollbarVerticalWnd * scrollV = scrollbar->GetVScrollbar();

	int x = scrollH->GetPosition() + xPos;
	int y = scrollV->GetPosition() + yPos;

	int nbElementByX = thumbnailSizeX / themeThumbnail.themeIcone.GetWidth();
	int numY = y / themeThumbnail.themeIcone.GetHeight();
	int numX = x / themeThumbnail.themeIcone.GetWidth();
	int numElement = numY * nbElementByX + numX;

	if (numElement >= pIconeList.size())
		return nullptr;

	return pIconeList.at(numElement);

}

void CThumbnailVertical::SetListeFile(const wxArrayString & listFile, const bool &showSelectButton)
{
    threadDataProcess = false;
    
    while (nbProcess > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    if(threadMain != nullptr)
        threadMain->join();

    InitScrollingPos();
    EraseThumbnailList();
    CreateOrLoadStorageFile();
    numActif = nullptr;
    numSelect = nullptr;

    int i = 0;
    int x = 0;
    int y = 0;

    int nbElementX = 0;
    int nbElementY = 0;

    int nbElementByRow = (width + positionXOld) / themeThumbnail.themeIcone.GetWidth();
    if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) <  (width + positionXOld))
        nbElementByRow++;

    int nbElementEnY = (int)listFile.size() / nbElementByRow;
    if (nbElementEnY * nbElementByRow < listFile.size())
        nbElementEnY++;

    for (wxString fileEntry : listFile)
    {
        CThumbnailDataSQL * thumbnailData = new CThumbnailDataSQL(fileEntry);
        //thumbnailData->SetStorage(pStorage->GetStoragePt());
        thumbnailData->SetNumElement(i);
        thumbnailData->SetNumPhotoId(i);

        CIcone * pBitmapIcone = new CIcone();
        pBitmapIcone->SetNumElement(i);
        pBitmapIcone->ShowSelectButton(showSelectButton);
        pBitmapIcone->SetData(thumbnailData);
        pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
        pBitmapIcone->SetWindowPos(x, y);

        pIconeList.push_back(pBitmapIcone);

        x += themeThumbnail.themeIcone.GetWidth(); nbElementX++;
        if (nbElementX == nbElementByRow)
        {
            nbElementX = 0;
            x = -posLargeur;
            nbElementY++;
            y += themeThumbnail.themeIcone.GetHeight();
        }
        
        i++;

    }
    SetNbFiles(i);

    thumbnailPos = 0;
    
    threadDataProcess = true;

}

void CThumbnailVertical::SetListeFile(const PhotosVector & photoVector, const bool &erase)
{

    if (erase)
    {
        InitScrollingPos();
        EraseThumbnailList();
    }


    numActif = nullptr;

    int i = 0;
    int x = 0;
    int y = 0;

    int nbElementX = 0;
    int nbElementY = 0;
    
    thumbnailPos = 0;

    int nbElementByRow = (width + positionXOld) / themeThumbnail.themeIcone.GetWidth();
    if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) <  (width + positionXOld))
        nbElementByRow++;

    int nbElementEnY = (int)photoVector.size() / nbElementByRow;
    if (nbElementEnY * nbElementByRow < photoVector.size())
        nbElementEnY++;

    for (CPhotos photo : photoVector)
    {
        CThumbnailDataSQL * thumbnailData = new CThumbnailDataSQL(photo.GetPath());
        thumbnailData->SetNumPhotoId(photo.GetId());
        thumbnailData->SetNumElement(i);

        CIcone * pBitmapIcone = new CIcone();
        pBitmapIcone->SetNumElement(i);
        pBitmapIcone->SetData(thumbnailData);
        pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
        pBitmapIcone->SetWindowPos(x, y);

        pIconeList.push_back(pBitmapIcone);

        x += themeThumbnail.themeIcone.GetWidth(); nbElementX++;
        if (nbElementX == nbElementByRow)
        {
            nbElementX = 0;
            x = -posLargeur;
            nbElementY++;
            y += themeThumbnail.themeIcone.GetHeight();
        }
        
        i++;
    }
    SetNbFiles(i);



	this->Refresh();
}

void CThumbnailVertical::RenderIcone(wxDC * deviceContext)
{
	int x = -posLargeur;
	int y = -posHauteur;

	int nbElementX = 0;
	int nbElementY = 0;

	int nbElementByRow = (width + positionXOld) / themeThumbnail.themeIcone.GetWidth();
	if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) <  (width + positionXOld))
		nbElementByRow++;

	int nbElementEnY = (int)pIconeList.size() / nbElementByRow;
	if (nbElementEnY * nbElementByRow < pIconeList.size())
		nbElementEnY++;
	

	for (CIcone * pBitmapIcone : pIconeList)
	{
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetWindowPos(x, y);
		pBitmapIcone->SetSizeIcone(themeThumbnail.themeIcone.GetRealWidth(), themeThumbnail.themeIcone.GetRealHeight());
		

		//if visible
		int left = x;
		int right = x + themeThumbnail.themeIcone.GetWidth();
		int top = y;
		int bottom = y + themeThumbnail.themeIcone.GetHeight();

		if ((right > 0 && left < width) && (top < height && bottom > 0))
		{
            if(numActif == nullptr && pBitmapIcone->GetNumElement() == 0)
            {
                numActif = pBitmapIcone;
                pBitmapIcone->SetActive(true);
            }
			RenderBitmap(deviceContext, pBitmapIcone);
		}
        else
        {
            pBitmapIcone->DestroyCache();
        }
        

		x += themeThumbnail.themeIcone.GetWidth(); nbElementX++;
		if (nbElementX == nbElementByRow)
		{
			nbElementX = 0;
			x = -posLargeur;
			nbElementY++;
			y += themeThumbnail.themeIcone.GetHeight();
		}
	}

	
}


void CThumbnailVertical::UpdateScroll()
{
    //bool update = false;
    int oldthumbnailSizeX = thumbnailSizeX;
    int oldthumbnailSizeY = thumbnailSizeY;
    
    thumbnailSizeX = 0;
    thumbnailSizeY = 0;
    
	//bool update = false;
	int nbElement = (int)pIconeList.size();

	if (nbElement > 0)
	{
		int nbElementByRow = (width + positionXOld) / themeThumbnail.themeIcone.GetWidth();
		if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) <  (width + positionXOld))
			nbElementByRow++;

		int nbElementEnY = (int)pIconeList.size() / nbElementByRow;
		if (nbElementEnY * nbElementByRow < pIconeList.size())
			nbElementEnY++;

		if (nbElement < nbElementByRow)
			nbElementByRow = nbElement;

		thumbnailSizeX = nbElementByRow * themeThumbnail.themeIcone.GetWidth();
		thumbnailSizeY = nbElementEnY * themeThumbnail.themeIcone.GetHeight();

		//scrollbar->SetControlSize(thumbnailSizeX, thumbnailSizeY);
		//scrollbar->SetPosition(posLargeur, posHauteur);
	}
    
    
    /*
    for (CInfosSeparationBar * infosSeparationBar : listSeparator)
    {
        int nbElement = (int)infosSeparationBar->listElement.size();
        
        int nbElementByRow = (width + positionXOld) / themeThumbnail.themeIcone.GetWidth();
        
        if (nbElement > 0 && nbElementByRow == 0)
        {
            float value = (float)(width + positionXOld) / (float)themeThumbnail.themeIcone.GetWidth();
            if (value > 0)
                nbElementByRow = 1;
        }
        
        if (nbElementByRow > 0)
        {
            if (nbElementByRow < 1)
                nbElementByRow = 1;
            
            if ((nbElementByRow * themeThumbnail.themeIcone.GetWidth()) < (width + positionXOld))
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
    */
    
    //bool refresh = false;
    if (pIconeList.size() >= 0)
    {
        int oldLargeur = posLargeur;
        int oldHauteur = posHauteur;
        
        float xRatio = 1.0;
        float yRatio = 1.0;
        
        if(oldthumbnailSizeX != 0)
            xRatio = (float)thumbnailSizeX / (float)oldthumbnailSizeX;
        
        if (oldthumbnailSizeY != 0)
            yRatio = (float)thumbnailSizeY / (float)oldthumbnailSizeY;
        
        float posX = (float)posLargeur * xRatio;
        float posY = (float)posHauteur * yRatio;
        
        scrollbar->SetControlSize(thumbnailSizeX, thumbnailSizeY);
        scrollbar->SetPosition(posX, posY);
        
        posLargeur = scrollbar->GetPosLargeur();
        posHauteur = scrollbar->GetPosHauteur();
        
        /*
         if(posLargeur != oldLargeur)
         refresh = true;
         if(posHauteur != oldHauteur)
         refresh = true;
         */
    }
    
    /*
     if(refresh)
     this->Refresh();
     */
}


void CThumbnailVertical::CalculPosHorizontal(const int &numItem)
{
	//Calcul NbElement By View
	int nbElement = (width / themeThumbnail.themeIcone.GetWidth());
	if ((nbElement * themeThumbnail.themeIcone.GetWidth()) < width)
		nbElement++;

	if (nbElement > pIconeList.size())
		nbElement = (int)pIconeList.size();

	int numFirstElement = posLargeur / themeThumbnail.themeIcone.GetWidth();
	int numLastElement = nbElement + numFirstElement;

	//nbElementToShow = pThumbnailDataList.size();

	if (numLastElement >= pIconeList.size())
	{
		numLastElement = (int)pIconeList.size() - 1;
	}

	if (numFirstElement < 0)
		numFirstElement = 0;

	//Obtention du numéro du dernier élément
	CIcone * pBitmapIconeLast = pIconeList.at(numLastElement);
	CIcone * pBitmapIconeFirst = pIconeList.at(numFirstElement);
	//CThumbnailWindowData * pThumbnailData = pBitmapIconeLast->GetData();

	int numElementMax = pBitmapIconeLast->GetData()->GetNumElement();
	int numElementMin = pBitmapIconeFirst->GetData()->GetNumElement();

	CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();

	//numSelect = numItem;
	if (!scrollH->IsMoving())
	{
		if (numItem >= numElementMax)
		{
			//Déplacement 
			int positionItem = posLargeur + ((numItem - numElementMax) + 1) * themeThumbnail.themeIcone.GetWidth();
			scrollH->SetPosition(positionItem);
			posLargeur = positionItem;

		}
		else if (numItem < numElementMin)
		{
			int positionItem = posLargeur - (numElementMin - numItem) * themeThumbnail.themeIcone.GetWidth();
			scrollH->SetPosition(positionItem);
			posLargeur = positionItem;
		}
		else if (numItem <= 0)
		{
			scrollH->SetPosition(0);
			posLargeur = 0;
		}
	}
}

void CThumbnailVertical::CalculPosVertical(const int &numItem)
{
	//Calcul NbElement By row
	int nbElement = (width / themeThumbnail.themeIcone.GetWidth());
	if ((nbElement * themeThumbnail.themeIcone.GetWidth()) < width)
		nbElement++;

	if (nbElement > pIconeList.size())
		nbElement = (int)pIconeList.size();

	int nbLigne = (height / themeThumbnail.themeIcone.GetHeight());
	if ((nbLigne * themeThumbnail.themeIcone.GetHeight()) < height)
		nbLigne++;

	int numFirstElement = (posHauteur / themeThumbnail.themeIcone.GetHeight()) * nbElement;
	int numLastElement = (nbElement * nbLigne) +  numFirstElement;

	//nbElementToShow = pThumbnailDataList.size();

	if (numLastElement >= pIconeList.size())
	{
		numLastElement = (int)pIconeList.size() - 1;
	}

	if (numFirstElement < 0)
		numFirstElement = 0;

	//Obtention du numéro du dernier élément
	CIcone * pBitmapIconeLast = pIconeList.at(numLastElement);
	CIcone * pBitmapIconeFirst = pIconeList.at(numFirstElement);
	//CThumbnailWindowData * pThumbnailData = pBitmapIconeLast->GetData();

	int numElementMax = pBitmapIconeLast->GetData()->GetNumElement();
	int numElementMin = pBitmapIconeFirst->GetData()->GetNumElement();

	CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();

	//numSelect = numItem;
	if (!scrollH->IsMoving())
	{
		if (numItem >= numElementMax)
		{
			//Déplacement 
			int positionItem = posLargeur + ((numItem - numElementMax) + 1) * themeThumbnail.themeIcone.GetWidth();
			scrollH->SetPosition(positionItem);
			posLargeur = positionItem;

		}
		else if (numItem < numElementMin)
		{
			int positionItem = posLargeur - (numElementMin - numItem) * themeThumbnail.themeIcone.GetWidth();
			scrollH->SetPosition(positionItem);
			posLargeur = positionItem;
		}
		else if (numItem <= 0)
		{
			scrollH->SetPosition(0);
			posLargeur = 0;
		}
	}
}

