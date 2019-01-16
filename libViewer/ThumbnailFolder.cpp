#include "ThumbnailFolder.h"
#include "MainWindow.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <LibResource.h>
#include <ThumbnailDataSQL.h>
#include <SqlFindPhotos.h>
#include <SqlFindYear.h>
#include <SqlFindMonth.h>
#include <SqlFindDay.h>
#include <SqlFindLocalisation.h>
#include <ConvertUtility.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarVerticalWnd.h>
#include <ScrollbarWnd.h>
#include <SqlPhotosWithoutThumbnail.h>
using namespace Regards::Viewer;
using namespace Regards::Sqlite;

CThumbnailFolder::CThumbnailFolder(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailVertical(parent, id, statusBarInterface, themeThumbnail, testValidity)
{
	
	barseparationHeight = 40;
	widthThumbnail = 0;
	heightThumbnail = 0;
}


CThumbnailFolder::~CThumbnailFolder(void)
{

}

void CThumbnailFolder::SetActifItem(const int &numItem, const bool &move)
{
    this->SetFocus();
   
    if (numItem >= iconeList->GetNbElement())
        return;
        
    CIcone * numItemSelected = iconeList->GetElement(numItem);
    
    if(numItemSelected == nullptr)
        return;    
    
    //if(numItemSelected == numActif)
     //   return numSelect->GetData()->GetFilename();        
    
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
                
                //int windowHeight = this->GetHeight();
               // int windowWidth = this->GetWidth();
                //int iconeHeight = numActif->GetHeight();
                //int iconeWidth = numActif->GetWidth();
                
                //int xPos = (windowWidth - iconeWidth) / 2;
                //int yPos = (windowHeight - iconeHeight) / 2;
                
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
    bufferUpdate = true;
#ifdef __APPLE__
    this->CallRefresh(this);
#else
	this->FastRefresh(this);
#endif
}

void CThumbnailFolder::OnPictureClick(CThumbnailData * data)
{
    CMainWindow * mainWindow = (CMainWindow *)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ONPICTURECLICK);
		evt.SetExtraLong(data->GetNumPhotoId());
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
		//mainWindow->PictureClick(data->GetNumPhotoId(), false);

}


void CThumbnailFolder::AddSeparatorBar(const wxString &libelle, PhotosVector * photoVector, int &nbElement)
{
	CInfosSeparationBarExplorer * infosSeparationBar = new CInfosSeparationBarExplorer(themeThumbnail.themeSeparation);
	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetWidth(GetWindowWidth());

	for (auto i = 0; i < photoVector->size();i++)
	{
		CPhotos photo = photoVector->at(i);
		infosSeparationBar->listElement.push_back((int)iconeList->GetNbElement());

		CThumbnailDataSQL * thumbnailData = new CThumbnailDataSQL(photo.GetPath(), testValidity);
		thumbnailData->SetNumPhotoId(photo.GetId());
		thumbnailData->SetNumElement(nbElement++);

		CIcone * pBitmapIcone = new CIcone(nullptr);
		pBitmapIcone->ShowSelectButton(true);
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		iconeList->AddElement(pBitmapIcone);
	}

	if (photoVector->size() > 0)
		listSeparator.push_back(infosSeparationBar);
}

PhotosVector CThumbnailFolder::GetPhotoByLocalisation(PhotosVector * photoVector, const wxString & criteria, const wxString & localisation, int &index)
{
	PhotosVector listPhoto;
	for (auto index = 0; index < photoVector->size(); index++)
	{
		CPhotos photo = photoVector->at(index);
		if (localisation != "")
		{
			if (photo.GetCreateDate().find(criteria) == 0 && photo.GetGpsInfos().find(localisation) == 0)
				listPhoto.push_back(photo);
		}
		else
		{
			if (photo.GetCreateDate().find(criteria) == 0)
				listPhoto.push_back(photo);
		}
	}
	return listPhoto;
}

PhotosVector CThumbnailFolder::GetPhotoByCriteria(PhotosVector * photoVector, const wxString & criteria, int &index)
{
	PhotosVector listPhoto;
	for (auto index = 0; index < photoVector->size(); index++)
	{
		CPhotos photo = photoVector->at(index);
		if (photo.GetCreateDate().find(criteria) == 0)
			listPhoto.push_back(photo);
	}
	return listPhoto;
}


void CThumbnailFolder::ShowByYear(PhotosVector * photoVector, int &nbElement)
{
	vector<wxString> listOfYear;
	CSqlFindYear findYear;
	findYear.SearchUniqueCriteria(&listOfYear);
	int numElement = 0;

	for (wxString year : listOfYear)
	{
		if (year != L"")
		{
			PhotosVector listPhoto = GetPhotoByCriteria(photoVector, year, numElement);	
			AddSeparatorBar(year, &listPhoto, nbElement);
		}
	}
}

void CThumbnailFolder::ShowByMonth(PhotosVector * photoVector, int &nbElement)
{
	CSqlFindPhotos sqlFindPhotos;
	wxString listMonth = CLibResource::LoadStringFromResource(L"LBLMONTHNAME", 1);
	vector<wxString> MonthName = CConvertUtility::split(listMonth, ',');
	int imonth = 0;
	vector<wxString> listOfYear;
	CSqlFindYear findYear;
	CSqlFindMonth findMonth;
	findYear.SearchUniqueCriteria(&listOfYear);
	int numElement = 0;

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
					PhotosVector listPhoto = GetPhotoByCriteria(photoVector, year + L"." + month, numElement);
					imonth = atoi(CConvertUtility::ConvertToUTF8(month));
					AddSeparatorBar(MonthName.at(imonth - 1) + L" " + year, &listPhoto, nbElement);
				}
			}
		}
	}
}

int CThumbnailFolder::Dayofweek(const int & d,const int & m, const int & y)
{
	int year = y;
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	year -= m < 3;
	return (year + year / 4 - year / 100 + year / 400 + t[m - 1] + d) % 7;
}


void CThumbnailFolder::ShowByLocalisation(PhotosVector * photoVector, int &nbElement)
{
	CSqlFindPhotos sqlFindPhotos;
	barseparationHeight = 40;
	vector<wxString> listOfYear;
	CSqlFindYear findYear;
	CSqlFindMonth findMonth;
	CSqlFindDay findDay;
	wxString listMonth = CLibResource::LoadStringFromResource(L"LBLMONTHNAME", 1);
	wxString listDay = CLibResource::LoadStringFromResource(L"LBLDAYNAME", 1);
	wxString libellePhoto = CLibResource::LoadStringFromResource(L"LBLALLPHOTO", 1);
	vector<wxString> MonthName = CConvertUtility::split(listMonth, ',');
	vector<wxString> DayName = CConvertUtility::split(listDay, ',');
	CSqlFindLocalisation findLocalisation;
	int iyear = 0;
	int imonth = 0;
	int iday = 0;
	int ijour = 0;
	int numElement = 0;
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
							iyear = atoi(CConvertUtility::ConvertToUTF8(year));
							imonth = atoi(CConvertUtility::ConvertToUTF8(month));
							iday = atoi(CConvertUtility::ConvertToUTF8(day));
							ijour = Dayofweek(iday, imonth, iyear);
							wxString libelleDay = DayName.at(ijour) + L" " + day + L" " + MonthName.at(imonth - 1) + L" , " + year;

							vector<wxString> listOfLocalisation;

							findLocalisation.SearchUniqueCriteria(&listOfLocalisation, day, month, year);

                            for (wxString localisation : listOfLocalisation)
							{
								if (localisation != L"")
								{
									PhotosVector listPhoto = GetPhotoByLocalisation(photoVector, year + L"." + month + L"." + day, localisation,numElement);

									//sqlFindPhotos.SearchPhotos(&photoVector, localisation, );

									wxString libelleLocalisation = L"";
									vector<wxString> libelle = CConvertUtility::split(localisation, '.');
									if (libelle.size() == 1)
										AddSeparatorBar(DayName.at(ijour) + L" " + day + L" " + MonthName.at(imonth - 1) + L" , " + year + L"@" + localisation, &listPhoto, nbElement);
									else
									{
										for (auto j = (int)libelle.size() - 1; j > 0; j--)
										{
											libelleLocalisation.append(libelle.at(j));
											if (j > 1)
												libelleLocalisation.append(L", ");
										}
										AddSeparatorBar(DayName.at(ijour) + L" " + day + L" " + MonthName.at(imonth - 1) + L" , " + year + L"@" + libelleLocalisation, &listPhoto, nbElement);
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

void CThumbnailFolder::ShowByDay(PhotosVector * photoVector, int &nbElement)
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
	int numElement = 0;

	wxString listMonth = CLibResource::LoadStringFromResource(L"LBLMONTHNAME", 1);
	wxString listDay = CLibResource::LoadStringFromResource(L"LBLDAYNAME", 1);
	wxString libellePhoto = CLibResource::LoadStringFromResource(L"LBLALLPHOTO", 1);
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
							PhotosVector listPhoto = GetPhotoByCriteria(photoVector, year + L"." + month + L"." + day, numElement);
							iyear = atoi(CConvertUtility::ConvertToUTF8(year));
							imonth = atoi(CConvertUtility::ConvertToUTF8(month));
							iday = atoi(CConvertUtility::ConvertToUTF8(day));
							ijour = Dayofweek(iday, imonth, iyear);
							AddSeparatorBar(DayName.at(ijour) + L" " + day + L" " + MonthName.at(imonth - 1) + L" , " + year + L"@", &listPhoto, nbElement);
						}
					}
				}
			}
		}
	}
}

void CThumbnailFolder::InitTypeAffichage(PhotosVector * photoVector, const int &typeAffichage)
{
	threadDataProcess = false;

	/*
	while (nbProcess > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	if (threadMain != nullptr)
		threadMain->join();
	*/
	//---------------------------------
	//Sauvegarde de l'état
	//---------------------------------

	int numPhotoIdActif = 0;
	int numPhotoIdSelect = 0;

	vector<CThumbnailData *> listSelectItem;

	GetSelectItem(listSelectItem);

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

	int i = 0;
	int typeLocal = typeAffichage;



	if (typeLocal == SHOW_ALL)
	{
		wxString libellePhoto = CLibResource::LoadStringFromResource(L"LBLALLPHOTO", 1);
		AddSeparatorBar(libellePhoto, photoVector, i);
	}
	else if (typeLocal == SHOW_BYYEAR)
	{
		ShowByYear(photoVector, i);
	}
	else if (typeLocal == SHOW_BYMONTH)
	{
		ShowByMonth(photoVector, i);
	}
	else if (typeLocal == SHOW_BYLOCALISATION)
	{
		ShowByLocalisation(photoVector, i);
	}
	else if (typeLocal == SHOW_BYDAY)
	{
		ShowByDay(photoVector, i);
	}

	//m_lock.unlock();

	SetNbFiles(i);

	//---------------------------------
	//Application de l'tat
	//---------------------------------

	if (listSelectItem.size() > 0)
	{
		for (CThumbnailData * data : listSelectItem)
		{
			CIcone * icone = FindIcone(data->GetNumPhotoId());
			if (icone != nullptr)
			{
				icone->SetChecked(true);
				icone->SetSelected(true);
			}
		}
	}

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

	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();
    bufferUpdate = true;
#ifdef __APPLE__
    this->CallRefresh(this);
#else
	this->FastRefresh(this);
#endif
}

void CThumbnailFolder::Init(PhotosVector * photoVector, const int &typeAffichage)
{
    
	CSqlPhotosWithoutThumbnail sqlPhoto;
	sqlPhoto.GeneratePhotoList();

	if (photoVector != nullptr)
	{
		if (noVscroll)
			SetListeFile(photoVector);
		else
			InitTypeAffichage(photoVector, typeAffichage);
	}
	processIdle = true;	
}

void CThumbnailFolder::SetListeFile(PhotosVector * photoVector)
{
	threadDataProcess = false;

	EraseThumbnailList();

	numSelect = nullptr;
	numActif = nullptr;
	int i = 0;
	int x = 0;
	int y = 0;
	thumbnailPos = 0;

	for (CPhotos fileEntry : *photoVector)
	{
		wxString filename = fileEntry.GetPath();
		CThumbnailDataSQL * thumbnailData = new CThumbnailDataSQL(filename, testValidity);
		thumbnailData->SetNumPhotoId(fileEntry.GetId());
		thumbnailData->SetNumElement(i);


		CIcone * pBitmapIcone = new CIcone(nullptr);
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetWindowPos(x, y);

		iconeList->AddElement(pBitmapIcone);

		x += themeThumbnail.themeIcone.GetWidth();
		i++;

	}

	threadDataProcess = true;
	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();
    bufferUpdate = true;
    processIdle = true;	
#ifdef __APPLE__
    this->CallRefresh(this);
#else
	this->FastRefresh(this);
#endif
}

CIcone * CThumbnailFolder::FindIcone(const int &photoId)
{
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



CIcone * CThumbnailFolder::FindElementWithVScroll(const int &xPos, const int &yPos)
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


CInfosSeparationBar * CThumbnailFolder::FindSeparatorElement(const int &xPos, const int &yPos)
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


void CThumbnailFolder::FindOtherElement(wxDC * dc, const int &x, const int &y)
{
	CInfosSeparationBar * separator = FindSeparatorElement(x, y);
	if (separator != nullptr)
	{
		CInfosSeparationBarExplorer * explorer = (CInfosSeparationBarExplorer *)separator;
		if (explorer != nullptr)
		{
			explorer->OnClick(x, y);
			//explorer->RenderIcone(dc);

            for (auto numElement : separator->listElement)		
			{
				CIcone * icone = iconeList->GetElement(numElement);
                if(icone != nullptr)
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
  // if(widthThumbnail != width || heightThumbnail != height)
  // {
        if (noVscroll)
            ResizeThumbnailWithoutVScroll();
        else
            ResizeThumbnailWithVScroll();
        
       widthThumbnail = GetWindowWidth();
       heightThumbnail = GetWindowHeight();
       
       UpdateScroll();
  // }
}

void CThumbnailFolder::ResizeThumbnailWithVScroll()
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

void CThumbnailFolder::ResizeThumbnailWithoutVScroll()
{

    int x = 0;
	int y = 0;

	//int nbPhoto = pIconeList.size();

    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);
			x += themeThumbnail.themeIcone.GetWidth();
		}
	}
    
    
}


void CThumbnailFolder::RenderIconeWithoutVScroll(wxDC * deviceContext)
{
    
	posLargeur = scrollbar->GetPosLargeur();

//#pragma omp parallel for
    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * pBitmapIcone = iconeList->GetElement(i);
        if (pBitmapIcone != nullptr)
        {
            pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
            wxRect rc = pBitmapIcone->GetPos();

            //if visible
            int left = rc.x - posLargeur;
            int right = rc.x + rc.width - posLargeur;

            if (right > 0 && left < GetWindowWidth())
                RenderBitmap(deviceContext, pBitmapIcone, -posLargeur, 0);
            else
                pBitmapIcone->DestroyCache();
        }
    }
}

CIcone * CThumbnailFolder::FindElement(const int &xPos, const int &yPos)
{
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


void CThumbnailFolder::RenderIconeWithVScroll(wxDC * deviceContext)
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

void CThumbnailFolder::UpdateScrollWithVScroll()
{
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

    printf("CThumbnailFolder::UpdateScrollWithVScroll old %d %d \n", oldthumbnailSizeX, oldthumbnailSizeY);
    printf("CThumbnailFolder::UpdateScrollWithVScroll new %d %d \n", thumbnailSizeX, thumbnailSizeY);

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
