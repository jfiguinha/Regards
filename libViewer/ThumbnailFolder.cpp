// ReSharper disable All
#include <header.h>
#include "ThumbnailFolder.h"
#include "MainWindow.h"
#include <LibResource.h>
#include <ThumbnailDataSQL.h>
#include <SqlFindPhotos.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarWnd.h>
#include <SqlPhotosWithoutThumbnail.h>
#include "TreatmentData.h"
#include "ViewerParam.h"
#include "ViewerParamInit.h"
#include <config_id.h>
#include <ParamInit.h>
#include "ThumbnailBuffer.h"
#include <RegardsConfigParam.h>
using namespace Regards::Viewer;
using namespace Regards::Sqlite;


CThumbnailFolder::CThumbnailFolder(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
                                   const bool& testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity)
{
	barseparationHeight = 40;
	widthThumbnail = 0;
	heightThumbnail = 0;
	nbVideoThumbnailProcess = 0;
	preprocess_thumbnail = true;
	Connect(wxEVENT_ENDGENTHUMB, wxCommandEventHandler(CThumbnailFolder::EndGenThumbnail));
}


CThumbnailFolder::~CThumbnailFolder(void)
{
	if (listSeparator != nullptr)
	{
		for (CInfosSeparationBar* infosSeparationBar : *listSeparator)
		{
			delete(infosSeparationBar);
		}

		listSeparator->clear();
		delete listSeparator;
	}
}

void CThumbnailFolder::OnPictureClick(CThumbnailData* data)
{
	CMainWindow* mainWindow = (CMainWindow*)this->FindWindowById(MAINVIEWERWINDOWID);
	if (mainWindow != nullptr && data != nullptr)
	{
		//mainWindow->PictureVideoClick(timePosition);
		wxCommandEvent evt(wxEVENT_ONPICTURECLICK);
		evt.SetExtraLong(data->GetNumPhotoId());
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}

CInfosSeparationBarExplorer* CThumbnailFolder::AddSeparatorBar(CIconeList* iconeListLocal, const wxString& libelle, int& nbElement)
{
	CInfosSeparationBarExplorer* infosSeparationBar = new CInfosSeparationBarExplorer(themeThumbnail.themeSeparation);
	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetWidth(GetWindowWidth());

	int local_nbElement = iconeListLocal->GetNbElement();
	int size = CThumbnailBuffer::GetVectorSize();
#ifndef USE_TBB_VECTOR
	for (auto i = 0; i < size; i++)
#else
	tbb::parallel_for(0, size, 1, [=](int i)
#endif
	{
		try
	{
		CPhotos photo = CThumbnailBuffer::GetVectorValue(i);
		CThumbnailDataSQL* thumbnailData = new CThumbnailDataSQL(photo.GetPath(), testValidity);
		thumbnailData->SetNumPhotoId(photo.GetId());
		thumbnailData->SetNumElement(local_nbElement + i);

		CIcone* pBitmapIcone = new CIcone();
		pBitmapIcone->ShowSelectButton(true);
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		iconeListLocal->AddElement(pBitmapIcone);
	}
	catch (...)
	{
		break;
	}
	}
#ifdef USE_TBB_VECTOR  
	);
#endif

	iconeListLocal->SortById();

	for (auto i = 0; i < size; i++)
	{
		infosSeparationBar->listElement.push_back(local_nbElement + i);
	}

	nbElement += size;

	return infosSeparationBar;
}


CInfosSeparationBarExplorer * CThumbnailFolder::AddSeparatorBar(PhotosVector * _pictures, CIconeList* iconeListLocal, const wxString& libelle, int& nbElement)
{
	CInfosSeparationBarExplorer* infosSeparationBar = new CInfosSeparationBarExplorer(themeThumbnail.themeSeparation);
	infosSeparationBar->SetTitle(libelle);
	infosSeparationBar->SetWidth(GetWindowWidth());

	int local_nbElement = iconeListLocal->GetNbElement();
	int size = _pictures->size();
#ifndef USE_TBB_VECTOR
	for (auto i = 0; i < size; i++)
#else
	tbb::parallel_for(0, size, 1, [=](int i)
#endif
	{
		try
		{
			CPhotos photo = _pictures->at(i);
			CThumbnailDataSQL* thumbnailData = new CThumbnailDataSQL(photo.GetPath(), testValidity);
			thumbnailData->SetNumPhotoId(photo.GetId());
			thumbnailData->SetNumElement(local_nbElement + i);

			CIcone* pBitmapIcone = new CIcone();
			pBitmapIcone->ShowSelectButton(true);
			pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			iconeListLocal->AddElement(pBitmapIcone);
		}
		catch(...)
		{
			break;
		}
	}
#ifdef USE_TBB_VECTOR  
    );
#endif

	iconeListLocal->SortById();

	for (auto i = 0; i < size; i++)
	{
		infosSeparationBar->listElement.push_back(local_nbElement + i);
	}

	nbElement += size;
	
	//if (size > 0)
	//	listSeparator.push_back(infosSeparationBar);
	return infosSeparationBar;
}

CIconeList * CThumbnailFolder::PrepareTypeAffichage(PhotosVector * _pictures, const int& typeAffichage,  InfosSeparationBarVector * listSeparator)
{
	CIconeList* iconeListLocal = new CIconeList();
	int i = 0;
	int typeLocal = typeAffichage;

	if (typeLocal == SHOW_ALL)
	{
		wxString libellePhoto = CLibResource::LoadStringFromResource(L"LBLALLPHOTO", 1);
		CInfosSeparationBarExplorer * infosSeparationBar = AddSeparatorBar(_pictures, iconeListLocal, libellePhoto, i);
		if (_pictures->size() > 0)
			listSeparator->push_back(infosSeparationBar);
	}
	else if (typeLocal == SHOW_BYYEAR)
	{
		CTreatmentDataYear dataYear;
		dataYear.MainTreatment(listSeparator, _pictures, iconeListLocal, this, i);
	}
	else if (typeLocal == SHOW_BYMONTH)
	{
		CTreatmentDataMonth dataMonth;
		dataMonth.MainTreatment(listSeparator, _pictures, iconeListLocal, this, i);
	}
	else if (typeLocal == SHOW_BYLOCALISATION)
	{
		CTreatmentDataLocalisation dataLocalisation;
		dataLocalisation.MainTreatment(listSeparator, _pictures, iconeListLocal, this, i);
	}
	else if (typeLocal == SHOW_BYDAY)
	{
		CTreatmentDataDay dataDay;
		dataDay.MainTreatment(listSeparator, _pictures, iconeListLocal, this, i);
	}


	return iconeListLocal;
}


void CThumbnailFolder::ApplyTypeAffichage(CIconeList* iconeListLocal, InfosSeparationBarVector* _listSeparator, int typeAffichage)
{
	CIconeList* oldIconeList = iconeList;
	//---------------------------------
	//Sauvegarde de l'état
	//---------------------------------
	vector<CThumbnailData*> listSelectItem;
	threadDataProcess = false;
	GetSelectItem(listSelectItem);

	InfosSeparationBarVector* old = listSeparator;
	
	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
		config->SetTypeAffichage(typeAffichage);
	}
	
	lockIconeList.lock();
	iconeList = iconeListLocal;
	listSeparator = _listSeparator;
	lockIconeList.unlock();

	//------------------------------------------------------------------
	//Cleaning old Element
	//------------------------------------------------------------------
	if (old != nullptr)
	{
		for (CInfosSeparationBar* infosSeparationBar : *old)
		{
			delete(infosSeparationBar);
		}

		old->clear();
		delete old;
	}
	

	if (oldIconeList != nullptr)
	{
		EraseThumbnailList(oldIconeList);
		oldIconeList = nullptr;
	}

	nbElementInIconeList = iconeList->GetNbElement();

	//---------------------------------
	//Application de l'tat
	//---------------------------------

	if (listSelectItem.size() > 0)
	{
		for (CThumbnailData* data : listSelectItem)
		{
			int itemId = GetNumItemById(data->GetNumPhotoId());
			CIcone* icone = iconeList->GetElement(itemId);
			if (icone != nullptr)
			{
				icone->SetChecked(true);
				icone->SetSelected(true);
			}
		}
	}

	AfterSetList();

	thumbnailPos = 0;

	threadDataProcess = true;

	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();

	needToRefresh = true;
}



void CThumbnailFolder::InitTypeAffichage(const int& typeAffichage)
{
	CIconeList* iconeListLocal = new CIconeList();
	CIconeList* oldIconeList = iconeList;
	//---------------------------------
	//Sauvegarde de l'état
	//---------------------------------
	vector<CThumbnailData*> listSelectItem;
	threadDataProcess = false;
	GetSelectItem(listSelectItem);

	InfosSeparationBarVector* _listSeparator = new InfosSeparationBarVector();
	InfosSeparationBarVector* old = listSeparator;

	int i = 0;
	int typeLocal = typeAffichage;
	int size = CThumbnailBuffer::GetVectorSize();

	if (typeLocal == SHOW_ALL)
	{
		wxString libellePhoto = CLibResource::LoadStringFromResource(L"LBLALLPHOTO", 1);
		CInfosSeparationBarExplorer* infosSeparationBar = AddSeparatorBar(iconeListLocal, libellePhoto, i);
		if (size > 0)
			_listSeparator->push_back(infosSeparationBar);
	}
	else if (typeLocal == SHOW_BYYEAR)
	{
		CTreatmentDataYear dataYear;
		dataYear.MainTreatment(_listSeparator, iconeListLocal, this, i);
	}
	else if (typeLocal == SHOW_BYMONTH)
	{
		CTreatmentDataMonth dataMonth;
		dataMonth.MainTreatment(_listSeparator, iconeListLocal, this, i);
	}
	else if (typeLocal == SHOW_BYLOCALISATION)
	{
		CTreatmentDataLocalisation dataLocalisation;
		dataLocalisation.MainTreatment(_listSeparator, iconeListLocal, this, i);
	}
	else if (typeLocal == SHOW_BYDAY)
	{
		CTreatmentDataDay dataDay;
		dataDay.MainTreatment(_listSeparator, iconeListLocal, this, i);
	}


	CMainParam* config = CMainParamInit::getInstance();
	if (config != nullptr)
	{
		config->SetTypeAffichage(typeAffichage);
	}


	lockIconeList.lock();
	iconeList = iconeListLocal;
	listSeparator = _listSeparator;
	lockIconeList.unlock();

	//------------------------------------------------------------------
	//Cleaning old Element
	//------------------------------------------------------------------
	if (old != nullptr)
	{
		for (CInfosSeparationBar* infosSeparationBar : *old)
		{
			delete(infosSeparationBar);
		}

		old->clear();
		delete old;
	}

	

	if (oldIconeList != nullptr)
	{
		EraseThumbnailList(oldIconeList);
		oldIconeList = nullptr;
	}

	nbElementInIconeList = iconeList->GetNbElement();

	//---------------------------------
	//Application de l'tat
	//---------------------------------

	if (listSelectItem.size() > 0)
	{
		for (CThumbnailData* data : listSelectItem)
		{
			int itemId = GetNumItemById(data->GetNumPhotoId());
			CIcone* icone = iconeList->GetElement(itemId);
			if (icone != nullptr)
			{
				icone->SetChecked(true);
				icone->SetSelected(true);
			}
		}
	}

	AfterSetList();

	thumbnailPos = 0;

	threadDataProcess = true;

	widthThumbnail = 0;
	heightThumbnail = 0;
	ResizeThumbnail();

	needToRefresh = true;
}

void CThumbnailFolder::Init(const int& typeAffichage)
{
	if (noVscroll)
		SetListeFile();
	else
		InitTypeAffichage(typeAffichage);

	processIdle = true;
}

void CThumbnailFolder::SetListeFile()
{
	CIconeList* iconeListLocal = new CIconeList();
	threadDataProcess = false;
	thumbnailPos = 0;
	int size = CThumbnailBuffer::GetVectorSize();
#ifndef USE_TBB_VECTOR
	for (auto i = 0; i < size; i++)
#else
    int size = photoVector->size();
	tbb::parallel_for(0, size, 1, [=](int i)    
#endif    
	{
		try
		{
			CPhotos fileEntry = CThumbnailBuffer::GetVectorValue(i);
			wxString filename = fileEntry.GetPath();
			CThumbnailDataSQL* thumbnailData = new CThumbnailDataSQL(filename, false);
			thumbnailData->SetNumPhotoId(fileEntry.GetId());
			thumbnailData->SetNumElement(i);


			CIcone* pBitmapIcone = new CIcone();
			pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(themeThumbnail.themeIcone.GetWidth() * i, 0);

			iconeListLocal->AddElement(pBitmapIcone);
		}
		catch(...)
		{
			break;
		}
	}
#ifdef USE_TBB_VECTOR  
    );
#endif


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
	return iconeList->FindElement(xPos, yPos, &_pf, this);
}


CInfosSeparationBar* CThumbnailFolder::FindSeparatorElement(const int& xPos, const int& yPos)
{
	int x = xPos + posLargeur;
	int y = yPos + posHauteur;

	if (listSeparator == nullptr)
		return nullptr;

	for (CInfosSeparationBar* separatorBar : *listSeparator)
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


void CThumbnailFolder::FindOtherElement( const int& x, const int& y)
{
	CInfosSeparationBar* separator = FindSeparatorElement(x, y);
	if (separator != nullptr)
	{
		CInfosSeparationBarExplorer* explorer = (CInfosSeparationBarExplorer*)separator;
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

	for (CInfosSeparationBar* infosSeparationBar : *listSeparator)
	{
		int nbElement_localX = 0;
		int nbElement_localY = 0;

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

	widthThumbnail = GetWindowWidth();
	heightThumbnail = GetWindowHeight();

	UpdateScroll();
}

bool CThumbnailFolder::ItemCompFonct(int xPos, int yPos, CIcone* icone, CWindowMain* parent) /* Définit une fonction. */
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
	return false;
}

CIcone* CThumbnailFolder::FindElement(const int& xPos, const int& yPos)
{
	pItemCompFonct _pf = &ItemCompFonct;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
}

void  CThumbnailFolder::ExecuteThumbnailGen(void * param)
{
	CThumbnailFolder* thumb = (CThumbnailFolder*)param;
	if(thumb != nullptr)
	{
		thumb->GenerateThumbnail();

		wxCommandEvent eventChange(wxEVENT_ENDGENTHUMB);
		thumb->GetEventHandler()->AddPendingEvent(eventChange);
	}
}

void CThumbnailFolder::EndGenThumbnail(wxCommandEvent& event)
{
	muThumb.lock();
	if(thread_thumbnail != nullptr)
	{
		thread_thumbnail->join();
		delete thread_thumbnail;
		thread_thumbnail = nullptr;
	}
	muThumb.unlock();

	nbVideoThumbnailProcess = 0;
}

void CThumbnailFolder::GenerateThumbnail()
{
	//for (const auto& [key, value] : listElementToShow)
	//{

	int i = 0;
	int nbValue = 0;
	do
	{
		CIcone* pBitmapIcone = nullptr;
		muVector.lock();
		nbValue = listElementToShow.size();
		if(i < nbValue)
			pBitmapIcone = iconeList->GetElement(listElementToShow[i]);
		muVector.unlock();

		if (pBitmapIcone != nullptr)
		{
			wxRect rc = pBitmapIcone->GetPos();
			int nbProcesseur = 1;
			if (CRegardsConfigParam* config = CParamInit::getInstance(); config != nullptr)
				nbProcesseur = config->GetThumbnailProcess();

			if (pBitmapIcone != nullptr)
			{
				if (CThumbnailData* pThumbnailData = pBitmapIcone->GetData(); pThumbnailData != nullptr)
				{
					const bool isProcess = pThumbnailData->IsProcess();
					//const bool isLoad = pThumbnailData->IsLoad();
					if (!isProcess) // && !isLoad)
					{
						if (nbProcess < (nbProcesseur + 2))
						{
							pThumbnailData->SetForceRefresh(true);
							ProcessThumbnail(pThumbnailData);
							pThumbnailData->SetIsProcess(true);
							nbProcess++;
						}
					}
				}
			}

		}
		else
			break;
		i++;

	} while (i < nbValue);
}

void CThumbnailFolder::VideoProcessThumbnail()
{
	muThumb.lock();
	if(thread_thumbnail == nullptr && listElementToShow.size() > 0)
	{
		thread_thumbnail = new std::thread(ExecuteThumbnailGen, this);
		nbVideoThumbnailProcess = 1;
	}
	muThumb.unlock();

}

void CThumbnailFolder::RenderIconeWithVScroll(wxDC* deviceContext)
{
	muVector.lock();
	listElementToShow.clear();
	muVector.unlock();

	if (listSeparator == nullptr)
		return;

	for (auto i = 0; i < listSeparator->size(); i++)
	{

		CInfosSeparationBar* infosSeparationBar = listSeparator->at(i);

		if (infosSeparationBar != nullptr)
		{
			int numElement = infosSeparationBar->listElement.at(0);
			CIcone* pBitmapIcone = iconeList->GetElement(numElement);
			wxRect rc = pBitmapIcone->GetPos();
			//int nbElement_localX = infosSeparationBar->GetNbElementX();
			int nbElement_localY = infosSeparationBar->GetNbElementY();

			//int width_size = nbElement_localX * themeThumbnail.themeIcone.GetWidth();
			int height_size =rc.y + (nbElement_localY * themeThumbnail.themeIcone.GetHeight()) - posHauteur;
			int start_height = rc.y - posHauteur;

			if (height_size > 0 && start_height < GetWindowHeight())
			{
				infosSeparationBar->Render(deviceContext, -posLargeur, -posHauteur);

				bool start = false;
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

							CThumbnailData* pThumbnailData = pBitmapIcone->GetData();
							const bool isProcess = pThumbnailData->IsProcess();
							if(!isProcess)
							{
								muVector.lock();
								listElementToShow.push_back(numElement);
								muVector.unlock();
							}
								
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

	for (CInfosSeparationBar* infosSeparationBar : *listSeparator)
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
			CControlSize* controlSize = new CControlSize();
			wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
			controlSize->controlWidth = thumbnailSizeX;
			controlSize->controlHeight = thumbnailSizeY;
			evt.SetClientData(controlSize);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}

		if (parent != nullptr)
		{
			wxSize* size = new wxSize();
			wxCommandEvent evt(wxEVENT_SETPOSITION);
			size->x = static_cast<int>(posX);
			size->y = static_cast<int>(posY);
			evt.SetClientData(size);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}

		posLargeur = posX;
		posHauteur = posY;
	}

	needToRefresh = true;
}
