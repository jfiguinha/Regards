#include "thumbnailCriteria.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <libPicture.h>
//#include <omp.h>
#include <SqlFindPhotos.h>
#include <SqlFindYear.h>
#include <SqlFindMonth.h>
#include <SqlFindDay.h>
#include <SqlFindLocalisation.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarVerticalWnd.h"
#include <ConvertUtility.h>
#include "CentralWnd.h"
#include <libResource.h>
#include <ThumbnailDataSQL.h>
#include <ScrollbarWnd.h>
#include <algorithm>
#include "MainWindow.h"
using namespace Regards::Window;
using namespace Regards::Sqlite;
using namespace Regards::Explorer;

CThumbnailCriteria::CThumbnailCriteria(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusbar, const CThemeThumbnail & themeThumbnail)
	: CThumbnail(parent, id, statusbar, themeThumbnail)
{
	typeAffichage = TYPEPHOTO;
	numCatalog = 0;
	numCategorie = 0;
	key = L"";

}

CThumbnailCriteria::~CThumbnailCriteria(void)
{
	for (CInfosSeparationBar * infosSeparationBar : listSeparator)
	{
		delete(infosSeparationBar);
	}
	listSeparator.clear();
}

int CThumbnailCriteria::GetId(const int &numItem)
{
	CIcone * pBitmapIcone = pIconeList.at(numItem);
	return pBitmapIcone->GetData()->GetNumPhotoId();
}

void CThumbnailCriteria::SetNbFiles(const int &nbFiles)
{
	//Mise à jour du répertoire des fichiers
	CMainWindow * mainWnd = (CMainWindow *)this->FindWindowById(MAINEXPLORERWINDOWID);
	if (mainWnd != nullptr)
	{
		mainWnd->SetText(0, "Nb files : " + to_string(nbFiles));
	}
}

void CThumbnailCriteria::OnPictureClick(CThumbnailData * data)
{
	CCentralWnd * centralWnd = (CCentralWnd *)this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (centralWnd != nullptr)
	{
		centralWnd->SetNumImage(data->GetNumElement(), data->GetFilename());
	}
}

int CThumbnailCriteria::GetTypeAffichage()
{
	return typeAffichage;
}

CIcone * CThumbnailCriteria::FindElement(const int &xPos, const int &yPos)
{
	int x = xPos;
	int y = yPos;

	for (CIcone * icone : pIconeList)
	{
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

void CThumbnailCriteria::FindOtherElement(wxDC * dc, const int &x, const int &y)
{
	CInfosSeparationBar * separator = FindSeparatorElement(x, y);
	if (separator != nullptr)
	{
		CInfosSeparationBarExplorer * explorer = (CInfosSeparationBarExplorer *)separator;
		if (explorer != nullptr)
		{
			explorer->OnClick(x, y);
			explorer->RenderIcone(dc);

			for (int numElement : separator->listElement)
			{
				CIcone * icone = pIconeList.at(numElement);
				if (explorer->GetSelected())
					icone->SetChecked(true);
				else
					icone->SetChecked(false);
				icone->RenderIcone(dc);
			}
		}
	}
}

CInfosSeparationBar * CThumbnailCriteria::FindSeparatorElement(const int &xPos, const int &yPos)
{
	int x = xPos;
	int y = yPos;

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

int CThumbnailCriteria::Dayofweek(int d, int m, int y)
{
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	y -= m < 3;
	return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

int CThumbnailCriteria::GetNbElement()
{
	return (int)pIconeList.size();
}

void CThumbnailCriteria::AddSeparatorBar(const wxString &libelle, PhotosVector * photoVector, int &nbElement)
{
	CInfosSeparationBarExplorer * infosSeparationBar = new CInfosSeparationBarExplorer(themeThumbnail.themeSeparation);
	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetWidth(width);

	for (CPhotos photo : *photoVector)
	{
		infosSeparationBar->listElement.push_back((int)pIconeList.size());

		CThumbnailDataSQL * thumbnailData = new CThumbnailDataSQL(photo.GetPath());
		thumbnailData->SetNumPhotoId(photo.GetId());
		thumbnailData->SetNumElement(nbElement++);

		CIcone * pBitmapIcone = new CIcone();
		pBitmapIcone->ShowSelectButton(true);
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pIconeList.push_back(pBitmapIcone);
	}


	listSeparator.push_back(infosSeparationBar);
}


void CThumbnailCriteria::ShowByYear(int &nbElement)
{
    CSqlFindPhotos sqlFindPhotos;
    vector<wxString> listOfYear;
    CSqlFindYear findYear;
    findYear.SearchUniqueCriteria(&listOfYear);
    
    for (wxString year : listOfYear)
    {
        if (year != L"")
        {
            PhotosVector photoVector;
            sqlFindPhotos.SearchPhotos(&photoVector, year);
            AddSeparatorBar(year, &photoVector, nbElement);
        }
    }
}

void CThumbnailCriteria::ShowByMonth(int &nbElement)
{
    CSqlFindPhotos sqlFindPhotos;
    wxString listMonth = CLibResource::LoadStringFromResource(L"LBLMONTHNAME",1);
    vector<wxString> MonthName = CConvertUtility::split(listMonth, ',');
    int imonth = 0;
    vector<wxString> listOfYear;
    CSqlFindYear findYear;
    CSqlFindMonth findMonth;
    findYear.SearchUniqueCriteria(&listOfYear);
    
    for (wxString year : listOfYear)
    {
        if (year != L"")
        {
            vector<wxString> listOfMonth;
            findMonth.SearchUniqueCriteria(&listOfMonth, year);
            for (wxString month : listOfMonth)
            {
                if (month != L"")
                {
                    PhotosVector photoVector;
                    sqlFindPhotos.SearchPhotos(&photoVector, year + L"." + month);
                    imonth = atoi(month);
                    AddSeparatorBar(MonthName.at(imonth - 1) + L" " + year, &photoVector, nbElement);
                }
            }
        }
    }
}

void CThumbnailCriteria::ShowByLocalisation(int &nbElement)
{
    CSqlFindPhotos sqlFindPhotos;
    barseparationHeight = 40;
    vector<wxString> listOfYear;
    CSqlFindYear findYear;
    CSqlFindMonth findMonth;
    CSqlFindDay findDay;
    wxString listMonth = CLibResource::LoadStringFromResource(L"LBLMONTHNAME",1);
    wxString listDay = CLibResource::LoadStringFromResource(L"LBLDAYNAME",1);
    wxString libellePhoto = CLibResource::LoadStringFromResource(L"LBLALLPHOTO",1);
    vector<wxString> MonthName = CConvertUtility::split(listMonth, ',');
    vector<wxString> DayName = CConvertUtility::split(listDay, ',');
    CSqlFindLocalisation findLocalisation;
    int iyear = 0;
    int imonth = 0;
    int iday = 0;
    int ijour = 0;
    
    findYear.SearchUniqueCriteria(&listOfYear);
    for (wxString year : listOfYear)
    {
        if (year != L"")
        {
            vector<wxString> listOfMonth;
            findMonth.SearchUniqueCriteria(&listOfMonth, year);
            for (wxString month : listOfMonth)
            {
                if (month != L"")
                {
                    vector<wxString> listOfDay;
                    findDay.SearchUniqueCriteria(&listOfDay, month, year);
                    for (wxString day : listOfDay)
                    {
                        if (day != L"")
                        {
                            iyear = atoi(year);
                            imonth = atoi(month);
                            iday = atoi(day);
                            ijour = Dayofweek(iday, imonth, iyear);
                            wxString libelleDay = DayName.at(ijour) + L" " + day + L" " + MonthName.at(imonth - 1) + L" , " + year;
                            
                            vector<wxString> listOfLocalisation;
                            
                            findLocalisation.SearchUniqueCriteria(&listOfLocalisation, day, month, year);
                            for (wxString localisation : listOfLocalisation)
                            {
                                if (localisation != L"")
                                {
                                    PhotosVector photoVector;
                                    
                                    sqlFindPhotos.SearchPhotos(&photoVector, localisation, year + L"." + month + L"." + day);
                                    
                                    wxString libelleLocalisation = L"";
                                    vector<wxString> libelle = CConvertUtility::split(localisation, '.');
                                    if (libelle.size() == 1)
                                        AddSeparatorBar(DayName.at(ijour) + L" " + day + L" " + MonthName.at(imonth - 1) + L" , " + year + L"@" + localisation, &photoVector, nbElement);
                                    else
                                    {
                                        for (int j = (int)libelle.size() - 1; j > 0; j--)
                                        {
                                            libelleLocalisation.append(libelle.at(j));
                                            if (j > 1)
                                                libelleLocalisation.append(L", ");
                                        }
                                        AddSeparatorBar(DayName.at(ijour) + L" " + day + L" " + MonthName.at(imonth - 1) + L" , " + year + L"@" + libelleLocalisation, &photoVector, nbElement);
                                    }
                                    
                                }
                            }
                        }
                        
                    }
                }
            }
        }
    }
}

void CThumbnailCriteria::ShowByDay(int &nbElement)
{
    CSqlFindPhotos sqlFindPhotos;
    vector<wxString> listOfYear;
    CSqlFindYear findYear;
    CSqlFindMonth findMonth;
    CSqlFindDay findDay;
    int iyear = 0;
    int imonth = 0;
    int iday = 0;
    int ijour = 0;
    wxString listMonth = CLibResource::LoadStringFromResource(L"LBLMONTHNAME",1);
    wxString listDay = CLibResource::LoadStringFromResource(L"LBLDAYNAME",1);
    wxString libellePhoto = CLibResource::LoadStringFromResource(L"LBLALLPHOTO",1);
    vector<wxString> MonthName = CConvertUtility::split(listMonth, ',');
    vector<wxString> DayName = CConvertUtility::split(listDay, ',');
    findYear.SearchUniqueCriteria(&listOfYear);
    for (wxString year : listOfYear)
    {
        if (year != L"")
        {
            vector<wxString> listOfMonth;
            
            findMonth.SearchUniqueCriteria(&listOfMonth, year);
            for (wxString month : listOfMonth)
            {
                if (month != L"")
                {
                    vector<wxString> listOfDay;
                    
                    findDay.SearchUniqueCriteria(&listOfDay, month, year);
                    
                    for (wxString day : listOfDay)
                    {
                        if (day != L"")
                        {
                            PhotosVector photoVector;
                            
                            sqlFindPhotos.SearchPhotos(&photoVector, year + L"." + month + L"." + day);
                            
                            iyear = atoi(year);
                            imonth = atoi(month);
                            iday = atoi(day);
                            ijour = Dayofweek(iday, imonth, iyear);
                            
                            AddSeparatorBar(DayName.at(ijour) + L" " + day + L" " + MonthName.at(imonth - 1) + L" , " + year + L"@", &photoVector, nbElement);
                        }
                    }
                }
            }
        }
    }
}

void CThumbnailCriteria::Init(const int &typeAffichage)
{
	threadDataProcess = false;

	while (nbProcess > 0)
	{ 
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
    
    if(threadMain != nullptr)
        threadMain->join();
    
    //---------------------------------
    //Sauvegarde de l'tat
    //---------------------------------

    int numPhotoIdActif = 0;
    int numPhotoIdSelect = 0;
    
    vector<CThumbnailData *> listSelectItem;
    
    GetSelectItem(listSelectItem);
    
    if(numSelect != nullptr)
    {
        CThumbnailData * data = numSelect->GetData();
        if(data != nullptr)
            numPhotoIdSelect = data->GetNumPhotoId();
    }
    
    if(numActif != nullptr)
    {
        CThumbnailData * data = numActif->GetData();
        if(data != nullptr)
            numPhotoIdActif = data->GetNumPhotoId();
    }

    
    InitScrollingPos();
    
	EraseThumbnailList();

	numSelect = nullptr;
	numActif = nullptr;
    
    

	for (CInfosSeparationBar * infosSeparationBar : listSeparator)
	{
		delete(infosSeparationBar);
	}
	listSeparator.clear();

	int i = 0;
	int typeLocal = typeAffichage;



	if (typeLocal == SHOW_ALL)
	{
        wxString libellePhoto = CLibResource::LoadStringFromResource(L"LBLALLPHOTO",1);
		PhotosVector photoVector;
		CSqlFindPhotos sqlFindPhotos;
		sqlFindPhotos.SearchPhotos(&photoVector);
		AddSeparatorBar(libellePhoto, &photoVector, i);
	}
    else if (typeLocal == SHOW_BYYEAR)
    {
        ShowByYear(i);
    }
    else if (typeLocal == SHOW_BYMONTH)
    {
        ShowByMonth(i);
    }
    else if (typeLocal == SHOW_BYLOCALISATION)
    {
        ShowByLocalisation(i);
    }
    else if (typeLocal == SHOW_BYDAY)
    {
        ShowByDay(i);
    }

	//m_lock.unlock();

	SetNbFiles(i);
    
    //---------------------------------
    //Application de l'tat
    //---------------------------------
    
    if(listSelectItem.size() > 0)
    {
        for(CThumbnailData * data : listSelectItem)
        {
            CIcone * icone = FindIcone(data->GetNumPhotoId());
            if(icone != nullptr)
            {
                icone->SetChecked(true);
                icone->SetSelected(true);
            }
        }
    }
    
    if(numPhotoIdSelect > 0)
    {
        CIcone * icone = FindIcone(numPhotoIdSelect);
        if(icone != nullptr)
        {
            icone->SetSelected(true);
        }
    }
    
    if(numPhotoIdActif > 0)
    {
        CIcone * icone = FindIcone(numPhotoIdActif);
        if(icone != nullptr)
        {
            icone->SetActive(true);
        }
    }
    
    thumbnailPos = 0;

	threadDataProcess = true;
	this->Refresh();
}

CIcone * CThumbnailCriteria::FindIcone(const int &photoId)
{
    for(CIcone * icone : pIconeList)
    {
        if(icone != nullptr)
        {
            CThumbnailData * data = icone->GetData();
            if(data != nullptr)
            {
                if(data->GetNumPhotoId() == photoId)
                {
                    return icone;
                }
            }

        }

    }
    return nullptr;
}

void CThumbnailCriteria::RenderIcone(wxDC * deviceContext)
{
	int x = -posLargeur;
	int y = -posHauteur;

	int nbElementX = 0;
	int nbElementY = 0;

	int nbElementByRow = 0;

	//Calcul du width max
    for (int i = 0;i < listSeparator.size();i++)
	{
		//int nbElement = infosSeparationBar->listElement.size();

		int elementByRow = (width + positionXOld) / themeThumbnail.themeIcone.GetWidth();
		if ((elementByRow * themeThumbnail.themeIcone.GetWidth()) <  (width + positionXOld))
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

		//infosSeparationBar->SetSizeIcone(width, barseparationHeight);
		infosSeparationBar->SetWidth(controlWidth);
		infosSeparationBar->SetWindowPos(x, y);
		infosSeparationBar->RenderIcone(deviceContext);

		y += infosSeparationBar->GetHeight();

		for (int numElement : infosSeparationBar->listElement)
		{
			CIcone * pBitmapIcone = pIconeList.at(numElement);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);

			//if visible
			int left = x;
			int right = x + themeThumbnail.themeIcone.GetWidth();
			int top = y;
			int bottom = y + themeThumbnail.themeIcone.GetHeight();

			if ((right > 0 && left < width) && (top < height && bottom > 0))
			{
				RenderBitmap(deviceContext, pBitmapIcone);
			}
            else
                pBitmapIcone->DestroyCache();

			x += themeThumbnail.themeIcone.GetWidth(); nbElementX++;
			if (nbElementX == nbElementByRow)
			{
				nbElementX = 0;
				x = -posLargeur;
				nbElementY++;
				y += themeThumbnail.themeIcone.GetHeight();
			}
		}

		if (nbElementX != 0)
		{
			nbElementX = 0;
			x = -posLargeur;
			nbElementY++;
			y += themeThumbnail.themeIcone.GetHeight();
		}

	}

}

void CThumbnailCriteria::UpdateScroll()
{
	//bool update = false;
    int oldthumbnailSizeX = thumbnailSizeX;
    int oldthumbnailSizeY = thumbnailSizeY;
    
	thumbnailSizeX = 0;
	thumbnailSizeY = 0;
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

