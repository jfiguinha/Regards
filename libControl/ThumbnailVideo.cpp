#include <header.h>
#include "ThumbnailVideo.h"
#include <Thumbnail.h>
#include <RegardsBitmap.h>
#include <ThumbnailDataStorage.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarWnd.h"
#include <FileUtility.h>
#include <libPicture.h>
#include <ImageLoadingFormat.h>
#include <SqlThumbnailVideo.h>
using namespace Regards::Control;
using namespace Regards::Window;

#define wxEVENT_UPDATEVIDEOTHUMBNAIL 1001

CThumbnailVideo::CThumbnailVideo(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailHorizontal(parent, id, statusBarInterface, themeThumbnail, testValidity)
{
	numItemSelected = -1;
	isThreadStart = false;
	Connect(wxEVENT_UPDATEVIDEOTHUMBNAIL, wxCommandEventHandler(CThumbnailVideo::UpdateVideoThumbnail));
}

CThumbnailVideo::~CThumbnailVideo(void)
{

}

int CThumbnailVideo::FindNumItem(const int &videoPos)
{
	int numItem = 0;
    int numElement = iconeList->GetNbElement();
	for (int i = 0;i < numElement;i++)
	{
        CIcone * icone = iconeList->GetElement(i);
		if (icone != nullptr)
		{
			CThumbnailData * data = icone->GetData();
            if(data != nullptr)
            {
                if (data->GetTimePosition() > videoPos)
                {
                    numItem--;
                    break;
                }
            }
		}
		numItem++;
	}

	if (numItem >= numElement)
		numItem = numElement - 1;

	return numItem;
}

void CThumbnailVideo::SetVideoPosition(const int64_t &videoPos)
{
	int numItem = 0;
	CIcone * pIcone = nullptr;
    int nbIconeElement = iconeList->GetNbElement();
	//wxWindowDC dc(this);

	if (nbIconeElement == 0)
		return;

	numItem = FindNumItem(videoPos);
	if (numItem == numItemSelected)
		return;
	
	if (numSelect != nullptr)
	{
		numSelect->SetSelected(false);
		//numSelect->RenderIcone(&dc);
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

	CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();
	if (!scrollH->IsMoving() && pIcone != nullptr)
	{
		wxRect rect = pIcone->GetPos();
		rect.x = posLargeur + rect.x;
		rect.y = posHauteur + rect.y;
		scrollH->SetPosition(rect.x); scrollH->Refresh();
		posLargeur = scrollH->GetPosition();
	}

	numSelect = pIcone;
	//numSelect->RenderIcone(&dc);
	numItemSelected = numItem;
    bufferUpdate = true;
	this->FastRefresh(this);
}

void CThumbnailVideo::LoadVideoThumbnail(CThreadVideoData * videoData)
{
    CSqlThumbnailVideo sqlThumbnailVideo;
    int nbResult = sqlThumbnailVideo.GetNbThumbnail(videoData->videoFilename);
    if(nbResult > 0)
    {
        for(int i= 0;i < nbResult;i++)
        {
            CImageVideoThumbnail * thumbnail = sqlThumbnailVideo.GetPictureThumbnail(videoData->videoFilename, i);
            if(thumbnail != nullptr)
                videoData->videoThumbnail.push_back(thumbnail);
        }
    }
    else
    {
        CLibPicture libPicture;
        videoData->videoThumbnail = libPicture.LoadAllVideoThumbnail(videoData->videoFilename);
    }
    wxCommandEvent event(wxEVENT_UPDATEVIDEOTHUMBNAIL);
    event.SetClientData(videoData);
    wxPostEvent(videoData->window, event);
}

void CThumbnailVideo::UpdateVideoThumbnail(wxCommandEvent& event)
{
	//int width = 0;
	//int height = 0;
    int nbIconeElement = iconeList->GetNbElement();
	numItemSelected = -1;
	numSelect = nullptr;
	numActif = nullptr;
	//Création de la liste des icones
	//int x = 0;
	//int y = 0;

    thumbnailPos = 0;
	
	CThreadVideoData * videoData = (CThreadVideoData *)event.GetClientData();
	
	if(videoFilename == videoData->videoFilename)
	{
		if (videoData->videoThumbnail.size() > 0)
		{
			for(int i = 0;i < nbIconeElement;i++)
			{
				CImageVideoThumbnail * thumbnail = videoData->videoThumbnail[i];
				CIcone * pBitmapIcone = iconeList->GetElement(i);
                if(pBitmapIcone != nullptr)
                {
                    CThumbnailDataStorage * thumbnailData = (CThumbnailDataStorage *)pBitmapIcone->GetData();
                    if(thumbnailData != nullptr)
                    {
                        thumbnailData->SetBitmap(thumbnail->image);
                        thumbnailData->SetTimePosition(thumbnail->timePosition);
                        pBitmapIcone->DestroyCache();
                    }
                }
			}
		}
	}
	

	for (auto j = 0; j < videoData->videoThumbnail.size(); j++)
	{
		CImageVideoThumbnail * thumbnail = videoData->videoThumbnail[j];
		if(thumbnail != nullptr)
			delete thumbnail;
	}

	//delete videoData->videoThumbnail;

	if (videoData->myThread != nullptr)
		videoData->myThread->join();

	delete videoData->myThread;

	

	isThreadStart = false;

	
	if(videoFilename == videoData->videoFilename)
	{
		wxWindow * panelThumbnail = (wxWindow *)this->FindWindowById(THUMBNAILVIDEOPANEL);
		wxCommandEvent * eventUpdate = new wxCommandEvent(wxEVENT_ONSTOPLOADINGPICTURE);
		wxQueueEvent(panelThumbnail, eventUpdate);
        
        
	}
	else
	{
	
		StartThreadVideoLoading(videoFilename);
	}
	
	delete videoData;

    bufferUpdate = true;
    
    if(videoFilename == videoData->videoFilename)
        this->FastRefresh(this);
}

void CThumbnailVideo::InitWithDefaultPicture(const wxString & szFileName)
{
	CLibPicture libPicture;
	//int rotation = 0;
	EraseThumbnailList();
	vector<CImageVideoThumbnail *> listThumbnail = libPicture.LoadDefaultVideoThumbnail(szFileName);
	//Création de la liste des icones
	int x = 0;
	int y = 0;

	EraseThumbnailList();

    int i = 0;
    for (auto j = 0; j < 100; j += 5, i++)
    {
        CImageVideoThumbnail * thumbnail = listThumbnail[i];
        CThumbnailDataStorage * thumbnailData = new CThumbnailDataStorage(szFileName);
        //thumbnailData->SetStorage(nullptr);
        thumbnailData->SetNumPhotoId(i);
        thumbnailData->SetNumElement(i);
        thumbnailData->SetTypeElement(TYPEVIDEO);
        thumbnailData->SetPercent(j);
        thumbnailData->SetTimePosition(thumbnail->timePosition);
        thumbnailData->SetBitmap(thumbnail->image);

        CIcone * pBitmapIcone = new CIcone(nullptr);
        pBitmapIcone->SetNumElement(i);
        pBitmapIcone->SetData(thumbnailData);
        pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
        pBitmapIcone->SetWindowPos(x, y);

        if (j == 0)
        {
            pBitmapIcone->SetSelected(true);
            numSelect = pBitmapIcone;
        }

        iconeList->AddElement(pBitmapIcone);

        x += themeThumbnail.themeIcone.GetWidth();

    }
    
	

	for (auto j = 0; j < listThumbnail.size(); j++)
	{
		CImageVideoThumbnail * thumbnail = listThumbnail[j];
		if(thumbnail != nullptr)
			delete thumbnail;
	}
    
    bufferUpdate = true;

}

void CThumbnailVideo::StartThreadVideoLoading(const wxString &videoFile)
{
	CThreadVideoData * videoData = new CThreadVideoData();
	videoData->videoFilename = videoFile;
	videoData->window = this;
	//thread * threadloadPicture = new thread(LoadVideoThumbnail, videoData);
	videoData->myThread = nullptr;
    LoadVideoThumbnail(videoData);
	isThreadStart = true;
}

void CThumbnailVideo::SetVideoThumbnail(const wxString &videoFile, vector<CImageVideoThumbnail *> * pictureThumbnail)
{
	if (videoFilename != videoFile)
	{
		InitScrollingPos();
		int x = 0;
		int y = 0;
		EraseThumbnailList();
		//int i = 0;
		for (auto j = 0; j < pictureThumbnail->size(); j++)
		{
			CImageVideoThumbnail * thumbnail = pictureThumbnail->at(j);
			CThumbnailDataStorage * thumbnailData = new CThumbnailDataStorage(videoFile);
			//thumbnailData->SetStorage(nullptr);
			thumbnailData->SetNumPhotoId(j);
			thumbnailData->SetNumElement(j);
			thumbnailData->SetTypeElement(TYPEVIDEO);
			thumbnailData->SetPercent(thumbnail->percent);
			thumbnailData->SetTimePosition(thumbnail->timePosition);
			thumbnailData->SetBitmap(thumbnail->image);

			CIcone * pBitmapIcone = new CIcone(nullptr);
			pBitmapIcone->SetNumElement(j);
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);

			if (j == 0)
			{
				pBitmapIcone->SetSelected(true);
				numSelect = pBitmapIcone;
			}

			iconeList->AddElement(pBitmapIcone);

			x += themeThumbnail.themeIcone.GetWidth();
		}
		videoFilename = videoFile;
            
        bufferUpdate = true;      
	}


}

void CThumbnailVideo::SetVideoFile(const wxString &videoFile)
{
	if(videoFilename != videoFile)
	{
		InitScrollingPos();
		InitWithDefaultPicture(videoFile);
        StartThreadVideoLoading(videoFile);
		videoFilename = videoFile;	
        
        this->FastRefresh(this);
	}

    bufferUpdate = true;
}
