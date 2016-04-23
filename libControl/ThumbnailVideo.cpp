#include <Thumbnail.h>
#include <RegardsBitmap.h>
#include <videothumbnail.h>
#include "ThumbnailVideo.h"
#include <ThumbnailDataStorage.h>
#include "ScrollbarHorizontalWnd.h"
#include "ScrollbarWnd.h"
#include <FileUtility.h>
#include <libPicture.h>
using namespace Regards::Window;
using namespace Regards::Control;

CThumbnailVideo::CThumbnailVideo(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeThumbnail & themeThumbnail)
	: CThumbnailHorizontal(parent, id, statusBarInterface, themeThumbnail)
{

}

CThumbnailVideo::~CThumbnailVideo(void)
{

}

int CThumbnailVideo::FindNumItem(const int &videoPos)
{
	int numItem = 0;
	for (CIcone * icone : pIconeList)
	{
		if (icone != nullptr)
		{
			CThumbnailData * data = icone->GetData();
			if (data->GetTimePosition() > videoPos)
			{
				numItem--;
				break;
			}
		}
		numItem++;
	}

	if (numItem >= pIconeList.size())
		numItem = (int)pIconeList.size() - 1;

	return numItem;
}

void CThumbnailVideo::SetVideoPosition(const int64_t &videoPos)
{
	int numItem = 0;
	CIcone * pIcone = nullptr;
	wxWindowDC dc(this);

	if (pIconeList.size() == 0)
		return;

	numItem = FindNumItem(videoPos);
	if (numItem == numItemSelected)
		return;
	
	if (numSelect != nullptr)
	{
		numSelect->SetSelected(false);
		//numSelect->RenderIcone(&dc);
	}

	pIcone = pIconeList.at(numItem);
	pIcone->SetSelected(true);

	//Calcul NbElement By View
	int nbElement = (width / themeThumbnail.themeIcone.GetWidth());
	if ((nbElement * themeThumbnail.themeIcone.GetWidth()) < width)
		nbElement++;

	int numFirstElement = posLargeur / themeThumbnail.themeIcone.GetWidth();
	int numLastElement = nbElement + numFirstElement;

	//nbElementToShow = pThumbnailDataList.size();

	if (numLastElement > (pIconeList.size() - 1))
		numLastElement = (int)pIconeList.size() - 1;

	if (numFirstElement < 0)
		numFirstElement = 0;

	if (numFirstElement > (pIconeList.size() - 1))
		numFirstElement = (int)pIconeList.size() - 1;

	//Obtention du numéro du dernier élément
	//CIcone * pBitmapIconeLast = pIconeList.at(numLastElement);
	//CIcone * pBitmapIconeFirst = pIconeList.at(numFirstElement);
	//CThumbnailWindowData * pThumbnailData = pBitmapIconeLast->GetData();

	//int numElementMax = pBitmapIconeLast->GetData()->GetNumElement();
	//int numElementMin = pBitmapIconeFirst->GetData()->GetNumElement();

	CScrollbarHorizontalWnd * scrollH = scrollbar->GetHScrollbar();
	if (!scrollH->IsMoving())
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
	this->Refresh();
}

void CThumbnailVideo::SetVideoFile(const wxString &videoFile)
{

	CLibPicture libPicture;
	InitScrollingPos();
	EraseThumbnailList();
	CreateOrLoadStorageFile();
	thumbnailPos = 0;
	int width = 0;
	int height = 0;
	numItemSelected = -1;
	numSelect = nullptr;
	numActif = nullptr;
	//Création de la liste des icones
	int x = 0;
	int y = 0;



	CVideoThumbnail * videoThumbnail = libPicture.LoadAllVideoThumbnail(videoFile, width, height);

	if (videoThumbnail != nullptr)
	{

		int i = 0;
		for (int j = 0; j <= 100; j += 5, i++)
		{
			CThumbnailDataStorage * thumbnailData = new CThumbnailDataStorage(videoFile);
			//thumbnailData->SetStorage(nullptr);
			thumbnailData->SetNumPhotoId(i);
			thumbnailData->SetNumElement(i);
			thumbnailData->SetTypeElement(TYPEVIDEO);
			thumbnailData->SetPercent(j);
			thumbnailData->SetTimePosition(videoThumbnail->timePosition[i]);
			thumbnailData->SetBitmap(videoThumbnail->bitmap[i]);

			CIcone * pBitmapIcone = new CIcone();
			pBitmapIcone->SetNumElement(i);
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);

			if (j == 0)
			{
				pBitmapIcone->SetSelected(true);
				numSelect = pBitmapIcone;
			}

			pIconeList.push_back(pBitmapIcone);

			x += themeThumbnail.themeIcone.GetWidth();
		}


		delete videoThumbnail;
	}



	this->Refresh();
}
