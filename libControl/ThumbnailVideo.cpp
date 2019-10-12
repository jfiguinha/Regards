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
#include <picture_id.h>
using namespace Regards::Control;
using namespace Regards::Window;

#define wxTIMER_PROCESS 1001

CThumbnailVideo::CThumbnailVideo(wxWindow* parent, wxWindowID id, const CThemeThumbnail & themeThumbnail, const bool &testValidity)
	: CThumbnailHorizontal(parent, id, themeThumbnail, testValidity)
{
	numItemSelected = -1;
	process_end = true;
	processTimer = new wxTimer(this, wxTIMER_PROCESS);
	Connect(wxTIMER_PROCESS, wxEVT_TIMER, wxTimerEventHandler(CThumbnailVideo::OnTimerProcess), nullptr, this);
}

CThumbnailVideo::~CThumbnailVideo(void)
{
	if (processTimer->IsRunning())
		processTimer->Stop();

	delete(processTimer);
}

void CThumbnailVideo::OnTimerProcess(wxTimerEvent& event)
{
	if (!process_end)
	{
		processIdle = true;
	}
	else
		processTimer->Stop();
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
	Refresh();
}

void CThumbnailVideo::InitWithDefaultPicture(const wxString & szFileName, const int &size)
{
	int x = 0;
	int y = 0;
	int typeElement = TYPEVIDEO;
	//int rotation = 0;
	EraseThumbnailList();
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
			CImageVideoThumbnail * thumbnail = sqlThumbnailVideo.GetPictureThumbnail(szFileName, i);
			if (thumbnail == nullptr)
			{
				thumbnail = new CImageVideoThumbnail();
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
			thumbnailData->SetBitmap(thumbnail->image);

			CIcone * pBitmapIcone = new CIcone();
			pBitmapIcone->SetNumElement(i);
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);

			if (i == 0)
			{
				pBitmapIcone->SetSelected(true);
				numSelect = pBitmapIcone;
			}

			iconeList->AddElement(pBitmapIcone);

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
				numSelect = pBitmapIcone;
			}

			iconeList->AddElement(pBitmapIcone);

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


}

void CThumbnailVideo::ProcessThumbnailIdle()
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
				CImageVideoThumbnail * thumbnail = sqlThumbnailVideo.GetPictureThumbnail(videoFilename, i);
				if (thumbnail == nullptr)
				{
					thumbnail = new CImageVideoThumbnail();
					thumbnail->percent = ((float)i / (float)nbResult) * 100.0f;
					thumbnail->timePosition = i;
					thumbnail->image = libPicture.LoadPicture(videoFilename, true, i);
				}
				CIcone * pBitmapIcone = iconeList->GetElement(i);
				if (pBitmapIcone != nullptr)
				{
					CThumbnailDataStorage * thumbnailData = (CThumbnailDataStorage *)pBitmapIcone->GetData();
					if (thumbnailData != nullptr)
					{
						thumbnailData->SetBitmap(thumbnail->image);
						thumbnailData->SetTimePosition(thumbnail->timePosition);
						pBitmapIcone->DestroyCache();
					}
				}
				if (thumbnail != nullptr)
					delete thumbnail;
			}
			processIdle = false;
			process_end = true;
			Refresh();
		}
		else
		{
			processIdle = true;
		}
	}
}


void CThumbnailVideo::SetFile(const wxString &videoFile, const int &size)
{
	if(videoFilename != videoFile)
	{
		threadDataProcess = true;
		process_end = false;
		InitScrollingPos();
		InitWithDefaultPicture(videoFile, size);
		videoFilename = videoFile;	
		processTimer->Start(500);
        Refresh();
	}

}
