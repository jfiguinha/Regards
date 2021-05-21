#include <header.h>
#include "ThumbnailVideo.h"
#include <Thumbnail.h>
#include <ThumbnailDataStorage.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarWnd.h"
#include <libPicture.h>
#include <SqlThumbnailVideo.h>
#include <picture_id.h>
#include <SqlThumbnail.h>
#include <SqlPhotosWithoutThumbnail.h>
#include <LibResource.h>
using namespace Regards::Control;
using namespace Regards::Window;
using namespace Regards::Picture;
#define wxTIMER_PROCESS 1001

CThumbnailVideo::CThumbnailVideo(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailHorizontal(parent, id, themeThumbnail, testValidity)
{
	numItemSelected = -1;
	process_end = true;
	Connect(wxEVENT_ENDVIDEOTHUMBNAIL, wxCommandEventHandler(CThumbnailVideo::EndVideoThumbnail));
}

void CThumbnailVideo::EndVideoThumbnail(wxCommandEvent& event)
{
	ProcessThumbnail();
}

CThumbnailVideo::~CThumbnailVideo(void)
{
}

bool CThumbnailVideo::ItemCompFonct(int videoPos, int y, CIcone * icone, CWindowMain * parent)   /* Définit une fonction. */
{
	if (icone != nullptr && parent != nullptr)
	{
		CThumbnailData * data = icone->GetData();
		if (data != nullptr)
		{
			if (data->GetTimePosition() > videoPos)
			{
				return true;
			}
		}
	}
	return false;
}

int CThumbnailVideo::FindNumItem(const int &videoPos)
{
	int numItem = 0;
	pItemCompFonct _pf = &ItemCompFonct;
	CIcone * icone = iconeList->FindElement(videoPos, 0, &_pf, this);
	if (icone != nullptr)
	{
		numItem = icone->GetNumElement() - 1;
	}
	return numItem;
}

void CThumbnailVideo::SetVideoPosition(const int64_t &videoPos)
{
	int numItem = 0;
	CIcone * pIcone = nullptr;
    int nbIconeElement = nbElementInIconeList;
	//wxClientDC dc(this);

	if (nbIconeElement == 0)
		return;

	numItem = FindNumItem(videoPos);
	if (numItem == numItemSelected)
		return;
	
	if (numSelectPhotoId != -1)
	{
		CIcone* numSelect = GetIconeById(numSelectPhotoId);
		if(numSelect != nullptr)
			numSelect->SetSelected(false);
	}

	pIcone = iconeList->GetElement(numItem);
    if(pIcone != nullptr)
    {
        pIcone->SetSelected(true);
    }

	//Calcul NbElement By View
	int nbElement = (GetWindowWidth() / themeThumbnail.themeIcone.GetWidth());
	if ((nbElement * themeThumbnail.themeIcone.GetWidth()) < GetWindowWidth())
		nbElement++;

	int numFirstElement = posLargeur / themeThumbnail.themeIcone.GetWidth();
	int numLastElement = nbElement + numFirstElement;

	//nbElementToShow = pThumbnailDataList.size();

	if (numLastElement > (nbIconeElement- 1))
		numLastElement = nbIconeElement - 1;

	if (numFirstElement < 0)
		numFirstElement = 0;

	if (numFirstElement > (nbIconeElement - 1))
		numFirstElement = nbIconeElement - 1;

	if (!isMoving && pIcone != nullptr)
	{
		wxRect rect = pIcone->GetPos();
		rect.x = posLargeur + rect.x;
		rect.y = posHauteur + rect.y;
        
		wxWindow * parent = this->GetParent();

		if (parent != nullptr)
		{
			wxSize * size = new wxSize();
			wxCommandEvent evt(wxEVENT_SETPOSITION);

			posHauteur = max((rect.y - this->GetWindowHeight() / 2), 0);
			posLargeur = max((rect.x - this->GetWindowWidth() / 2), 0);

			size->x = posLargeur;
			size->y = posHauteur;
			evt.SetClientData(size);
			parent->GetEventHandler()->AddPendingEvent(evt);
		}

		//posLargeur = rect.x;
	}

	numSelectPhotoId = iconeList->GetPhotoId(numItem);
	//numSelect->RenderIcone(&dc);
	numItemSelected = numItem;
	Refresh();
}

void CThumbnailVideo::InitWithDefaultPicture(const wxString & szFileName, const int &size)
{
	int x = 0;
	int y = 0;
	int typeElement = TYPEVIDEO;
	threadDataProcess = false;
	CIconeList* iconeListLocal = new CIconeList();
	CIconeList* oldIconeList = nullptr;

	CLibPicture libPicture;
	int iFormat = libPicture.TestImageFormat(szFileName);
	if (iFormat == PDF || iFormat == TIFF)
		typeElement = TYPEMULTIPAGE;

	CSqlThumbnailVideo sqlThumbnailVideo;
	int nbResult = sqlThumbnailVideo.GetNbThumbnail(szFileName);
	if (nbResult > 0)
	{
		for (int i = 0; i < nbResult; i++)
		{
			CImageVideoThumbnail * thumbnail = new CImageVideoThumbnail(); 
			sqlThumbnailVideo.GetPictureThumbnail(szFileName, i, thumbnail);
			if (thumbnail->image == nullptr)
			{
				thumbnail->percent = ((float)i / (float)nbResult) * 100.0f;
				thumbnail->timePosition = i;
				thumbnail->image = libPicture.LoadPicture(szFileName, true, i);
			}

			float percent = ((float)i / (float)size) * 100.0f;
			CThumbnailDataStorage * thumbnailData = new CThumbnailDataStorage(szFileName);
			//thumbnailData->SetStorage(nullptr);
			thumbnailData->SetNumPhotoId(i);
			thumbnailData->SetNumElement(i);
			thumbnailData->SetTypeElement(typeElement);
			thumbnailData->SetPercent(percent);
			if (typeElement == TYPEMULTIPAGE)
				thumbnailData->SetLibelle("Page : " + to_string(i+1) + "/" + to_string(nbResult));
			thumbnailData->SetTimePosition(thumbnail->timePosition);

			try
			{
				if (thumbnail->image != nullptr)
					thumbnailData->SetBitmap(thumbnail->image);
			}
			catch (...)
			{
				printf("toto");
			}


			CIcone * pBitmapIcone = new CIcone();
			pBitmapIcone->SetNumElement(i);
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);

			if (i == 0)
			{
				pBitmapIcone->SetSelected(true);
				numSelectPhotoId = i;
			}

			iconeListLocal->AddElement(pBitmapIcone);

			x += themeThumbnail.themeIcone.GetWidth();

			if (thumbnail != nullptr)
				delete thumbnail;
		}
		processIdle = false;
	}
	else
	{
		CLibPicture libPicture;

		vector<CImageVideoThumbnail *> listThumbnail = libPicture.LoadDefaultVideoThumbnail(szFileName, size);
		//Création de la liste des icones

		//int i = 0;
		for (auto j = 0; j < size; j++)
		{
			float percent = ((float)j / (float)size) * 100.0f;
			CImageVideoThumbnail * thumbnail = listThumbnail[j];
			CThumbnailDataStorage * thumbnailData = new CThumbnailDataStorage(szFileName);
			//thumbnailData->SetStorage(nullptr);
			thumbnailData->SetNumPhotoId(j);
			thumbnailData->SetNumElement(j);
			thumbnailData->SetTypeElement(typeElement);
			thumbnailData->SetPercent(percent);
			thumbnailData->SetTimePosition(thumbnail->timePosition);

			if (thumbnail->image == nullptr)
				thumbnail->image = libPicture.LoadPicture(CLibResource::GetPhotoCancel());

			thumbnailData->SetBitmap(thumbnail->image);
			if (typeElement == TYPEMULTIPAGE)
				thumbnailData->SetLibelle("Page : " + to_string(j+1) + "/" + to_string(size));
			CIcone * pBitmapIcone = new CIcone();
			pBitmapIcone->SetNumElement(j);
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);

			if (j == 0)
			{
				pBitmapIcone->SetSelected(true);
				numSelectPhotoId = j;
			}

			iconeListLocal->AddElement(pBitmapIcone);

			x += themeThumbnail.themeIcone.GetWidth();

		}



		for (auto j = 0; j < listThumbnail.size(); j++)
		{
			CImageVideoThumbnail * thumbnail = listThumbnail[j];
			if (thumbnail != nullptr)
				delete thumbnail;
		}

		processIdle = true;
	}

	

	lockIconeList.lock();
	oldIconeList = iconeList;
	iconeList = iconeListLocal;
	lockIconeList.unlock();

	nbElementInIconeList = iconeList->GetNbElement();

	EraseThumbnailList(oldIconeList);
	
	threadDataProcess = true;

	UpdateScroll();
}

void CThumbnailVideo::ResizeThumbnail()
{
	UpdateScroll();
	ProcessThumbnail();
}

void CThumbnailVideo::ProcessThumbnail()
{
	if (videoFilename != "")
	{
		CLibPicture libPicture;
		CSqlThumbnailVideo sqlThumbnailVideo;
		int nbResult = sqlThumbnailVideo.GetNbThumbnail(videoFilename);
		if (nbResult > 0)
		{
			for (int i = 0; i < nbResult; i++)
			{
				CImageVideoThumbnail * thumbnail = new CImageVideoThumbnail();
				sqlThumbnailVideo.GetPictureThumbnail(videoFilename, i, thumbnail);
				if (thumbnail->image == nullptr)
				{
					thumbnail = new CImageVideoThumbnail();
					thumbnail->percent = ((float)i / (float)nbResult) * 100.0f;
					thumbnail->timePosition = i;
					thumbnail->image = libPicture.LoadPicture(videoFilename, true, i);
				}
				//if (thumbnail->image == nullptr)
				//	thumbnail->image = libPicture.LoadPicture(CLibResource::GetPhotoCancel());

				CIcone * pBitmapIcone = iconeList->GetElement(i);
				if (pBitmapIcone != nullptr)
				{
					CThumbnailDataStorage * thumbnailData = (CThumbnailDataStorage *)pBitmapIcone->GetData();
					if (thumbnailData != nullptr)
					{
						if(thumbnail->image != nullptr)
							thumbnailData->SetBitmap(thumbnail->image);
						thumbnailData->SetTimePosition(thumbnail->timePosition);
						pBitmapIcone->DestroyCache();
					}
				}
				if (thumbnail != nullptr)
					delete thumbnail;
			}
		}
	}
	this->Refresh();
}


void CThumbnailVideo::EraseThumbnail(wxCommandEvent& event)
{
    TRACE();
    long value = event.GetExtraLong();
    if(value == 1)
    {
        CSqlThumbnailVideo sqlThumbnailvideo;
        sqlThumbnailvideo.DeleteThumbnail(videoFilename);
        
        CSqlThumbnail sqlThumbnail;
        sqlThumbnail.DeleteThumbnail(videoFilename);

        for (int i = 0;i < nbElementInIconeList;i++)
        {
            CIcone * pIcone = iconeList->GetElement(i);
            if(pIcone != nullptr)
            {
                CThumbnailData * pThumbnailData = pIcone->GetData();
                if (pThumbnailData != nullptr)
                {
                    pThumbnailData->InitLoadState();
                    wxString filename = pThumbnailData->GetFilename();
                    pThumbnailData->SetIsProcess(false);
                    pThumbnailData->SetIsLoading(false);
                    pIcone->DestroyCache();
                }
            }
        }
        
        CSqlPhotosWithoutThumbnail sqlPhoto;
        sqlPhoto.GeneratePhotoList();
        
        processIdle = true;
    }
	thumbnailPos = 0;

    process_end = false;
    InitScrollingPos();
    InitWithDefaultPicture(videoFilename, 20);
    //processTimer->Start(500);
    Refresh();
    
}
    

void CThumbnailVideo::SetFile(const wxString &videoFile, const int &size)
{
    process_end = false;
    InitScrollingPos();
    InitWithDefaultPicture(videoFile, size);
    videoFilename = videoFile;	
   // processTimer->Start(500);
    Refresh();
	processIdle = true;
}
