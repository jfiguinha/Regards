// ReSharper disable All
#include <header.h>
#include "ThumbnailVideo.h"
#include <Thumbnail.h>
#include <ThumbnailDataStorage.h>
#include "ScrollbarWnd.h"
#include <libPicture.h>
#include <SqlThumbnailVideo.h>
#include <SqlThumbnail.h>
#include <SqlPhotos.h>
#include <ImageVideoThumbnail.h>
#include <ConvertUtility.h>
#include <appcontext.h>
using namespace Regards::Control;
using namespace Regards::Window;
using namespace Regards::Picture;
#define WX_TIMER_PROCESS 1001
#define wxEVENT_ENDTHUMBNAIL 1002
#define wxEVENT_ENDUPDATEVIDEOTHUMBNAIL 1003

extern AppContext application_context;


struct ThumbnailVideoThread
{
	CThumbnailVideo* window;
	wxString filename;
	thread* threadVideo;
};


CThumbnailVideo::CThumbnailVideo(wxWindow* parent, const wxWindowID id, const CThemeThumbnail& themeThumbnail,
                                 const bool& testValidity)
	: CThumbnailHorizontal(parent, id, themeThumbnail, testValidity)
{
	numItemSelected = -1;
	process_end = true;
	nbProcess = 0;
	enableTimer = false;
	Connect(wxEVENT_REFRESHVIDEOTHUMBNAIL, wxCommandEventHandler(CThumbnailVideo::UpdateVideoThumbnail));
	Connect(wxEVENT_ICONEUPDATE, wxCommandEventHandler(CThumbnailVideo::UpdateThumbnailIcone));
	
}



void CThumbnailVideo::UpdateThumbnailIcone(wxCommandEvent& event)
{
	//printf("CMainWindow::UpdateThumbnailIcone \n");
	CThreadLoadingBitmap* threadLoadingBitmap = (CThreadLoadingBitmap*)event.GetClientData();
	if (threadLoadingBitmap == nullptr)
		return;

	nbProcess--;

	if (threadLoadingBitmap->filename == videoFilename)
		UpdateVideoThumbnail();
	else
	{
		GenerateThumbnail(videoFilename);
	}

	delete threadLoadingBitmap;
    
    
    needToRefresh = true;
		
}

bool CThumbnailVideo::ItemCompFonct(int videoPos, int y, CIcone * icone, CWindowMain* parent) /* Définit une fonction. */
{
	if (icone != nullptr && parent != nullptr)
	{
		CThumbnailData* data = icone->GetPtData();
		if (data != nullptr)
		{
			if (data->GetTimePosition() >= videoPos)
			{
				return true;
			}
		}
	}
	return false;
}

int CThumbnailVideo::FindNumItem(const int& videoPos)
{
	int numItem = -1;
	pItemCompFonct _pf = &ItemCompFonct;
	CIcone * icone = iconeList->FindElementByPosition(videoPos, 0, &_pf, this);
	if (icone != nullptr)
	{
		if (iFormat < 100)
			numItem = icone->GetNumElement();
		else
		{
			CThumbnailData* data = icone->GetPtData();
			if (data != nullptr)
			{
				if (data->GetTimePosition() == videoPos)
				{
					numItem = icone->GetNumElement();
				}
				else
				{
					numItem = icone->GetNumElement() - 1;
				}
			}
		}
	}
	return numItem;
}

void CThumbnailVideo::SetVideoPosition(const int64_t& videoPos)
{
	const int nbIconeElement = nbElementInIconeList;
	//wxClientDC dc(this);

	if (nbIconeElement == 0)
		return;

	int numItem = FindNumItem(videoPos);

	if (numItem == -1 && videoPos > 0)
	{
		CIcone* icone = iconeList->GetLastElement();
		if (icone != nullptr)
			numItem = icone->GetNumElement();
		else
			return;
	}


	if (numItem == numItemSelected && videoPos > 0)
	{
		return;
	}

	if (numSelectPhotoId != -1)
	{
		CIcone * numSelect = GetIconeById(numSelectPhotoId);
		if (numSelect != nullptr)
			numSelect->SetSelected(false);
	}

	CIcone * pIcone = iconeList->GetElement(numItem);
	if (pIcone != nullptr)
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

	if (numLastElement > (nbIconeElement - 1))
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

		wxWindow* parent = this->GetParent();

		if (parent != nullptr)
		{
			auto size = new wxSize();
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
	numItemSelected = numItem;
	needToRefresh = true;
	//this->Refresh();
}

void CThumbnailVideo::GenerateThumbnail(const wxString& szFileName)
{
	if (nbProcess == 0)
	{
		auto pLoadBitmap = new CThreadLoadingBitmap();
		pLoadBitmap->filename = szFileName;
		pLoadBitmap->window = this;
		pLoadBitmap->_thread = std::make_unique<std::thread>(LoadVideoThumbnail, pLoadBitmap);

		process_end = false;

		nbProcess++;
	}
}

void CThumbnailVideo::InitWithDefaultPicture(const wxString& szFileName, const int& size)
{
	int x = 0;
	int y = 0;
	int typeElement = TYPEVIDEO;
	threadDataProcess = false;
	auto iconeListLocal = new CIconeList();
	CLibPicture libPicture;

        
	if (iFormat < 100)
		typeElement = TYPEMULTIPAGE;

	CSqlThumbnailVideo sqlThumbnailVideo;
	int nbResult = sqlThumbnailVideo.GetNbThumbnail(szFileName);
	CSqlPhotos SqlPhotos;
	int photoId = SqlPhotos.GetPhotoId(szFileName);
	if (nbResult > 0)
	{
		for (int i = 0; i < nbResult; i++)
		{
			auto thumbnail = std::make_unique<CImageVideoThumbnail>();
			sqlThumbnailVideo.GetPictureThumbnail(photoId, szFileName, i, thumbnail.get());
			thumbnail->percent = (static_cast<float>(i) / static_cast<float>(nbResult)) * 100.0f;
			//thumbnail->timePosition = i;

			float percent = (static_cast<float>(i) / static_cast<float>(size)) * 100.0f;
			auto thumbnailData = new CThumbnailDataStorage(szFileName);
			//thumbnailData->SetStorage(nullptr);
			thumbnailData->SetNumPhotoId(i);
			thumbnailData->SetNumElement(i);
            
			thumbnailData->SetTypeElement(typeElement);
			thumbnailData->SetPercent(percent);
			if (typeElement == TYPEMULTIPAGE)
				thumbnailData->SetLibelle("Page : " + to_string(i + 1) + "/" + to_string(nbResult));
			thumbnailData->SetTimePosition(thumbnail->timePosition);

			try
			{
				if (!thumbnail->image.empty())
                {
					thumbnailData->SetBitmap(thumbnail->image);
                    thumbnailData->SetIsDefault(false);
                }
                else
                {
                    thumbnailData->SetIsDefault(true);
                }
			}
			catch (...)
			{
				//printf("toto");
			}


			auto pBitmapIcone = new CIcone(thumbnailData);
			pBitmapIcone->SetNumElement(i);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);

			if (i == 0)
			{
				pBitmapIcone->SetSelected(true);
				numSelectPhotoId = i;
			}

			iconeListLocal->AddElement(pBitmapIcone);

			x += themeThumbnail.themeIcone.GetWidth();
		}


	}
	else
	{
		//Start loading thumbnail
		GenerateThumbnail(szFileName);

		CLibPicture libPicture;
		std::vector<std::unique_ptr<CImageVideoThumbnail>>  listThumbnail = libPicture.LoadDefaultVideoThumbnail(szFileName, size);


		//int i = 0;
		for (auto j = 0; j < size; j++)
		{
			float percent = (static_cast<float>(j) / static_cast<float>(size)) * 100.0f;
			CImageVideoThumbnail* thumbnail = listThumbnail[j].get();
			auto thumbnailData = new CThumbnailDataStorage(szFileName);
			//thumbnailData->SetStorage(nullptr);
			thumbnailData->SetNumPhotoId(j);
			thumbnailData->SetNumElement(j);
			thumbnailData->SetTypeElement(typeElement);
			thumbnailData->SetPercent(percent);
			thumbnailData->SetTimePosition(thumbnail->timePosition);

			if (thumbnail->image.empty())
			{
				thumbnail->image = application_context.GetDefaultPicture();
			}

			thumbnailData->SetBitmap(thumbnail->image);
			if (typeElement == TYPEMULTIPAGE)
				thumbnailData->SetLibelle("Page : " + to_string(j + 1) + "/" + to_string(size));
                
			auto pBitmapIcone = new CIcone(thumbnailData);
			pBitmapIcone->SetNumElement(j);
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

        listThumbnail.clear();



	}

	auto old = std::move(iconeList);
	iconeList.reset(iconeListLocal);

	nbElementInIconeList = iconeList->GetNbElement();

	old->EraseThumbnailListWithIcon();


	threadDataProcess = true;

	UpdateScroll();

	processThumbnailVideo = true;

	//nbElementInIconeList = 1;

	needToRefresh = true;
}

void CThumbnailVideo::UpdateVideoThumbnail(const wxString& videoFile)
{
	if (videoFilename == videoFile)
		UpdateVideoThumbnail();
	else
		process_end = true;

    needToRefresh = true;
}

void CThumbnailVideo::LoadVideoThumbnail(void * param)
{
	CLibPicture libPicture;
	auto threadLoadingBitmap = static_cast<CThreadLoadingBitmap*>(param);
	if (threadLoadingBitmap == nullptr)
		return;


	std::vector<std::unique_ptr<CImageVideoThumbnail>> listVideo = libPicture.LoadAllVideoThumbnail(threadLoadingBitmap->filename, true, true);


	if (listVideo.size() > 0)
	{
		CSqlThumbnailVideo sqlThumbnailVideo;
		CSqlPhotos SqlPhotos;
		wxString filename = threadLoadingBitmap->filename; // bitmap->image->GetFilename();
		int photoId = SqlPhotos.GetPhotoId(filename);
		if (photoId != -1)
		{
			for (int i = 0; i < listVideo.size(); i++)
			{
				CImageVideoThumbnail* bitmap = listVideo[i].get();


				if (!bitmap->image.empty())
				{
					wxString localName = sqlThumbnailVideo.InsertThumbnail(photoId, filename, bitmap->image.size().width,
						bitmap->image.size().height, i, bitmap->rotation, bitmap->percent,
						bitmap->timePosition);

					cv::imwrite(CConvertUtility::ConvertToStdString(localName), bitmap->image);
					//bitmap->image.SaveFile(localName, wxBITMAP_TYPE_JPEG);
				}


				if (i == 0)
					threadLoadingBitmap->bitmapIcone = bitmap->image;

			}
		}

		threadLoadingBitmap->isAnimationOrVideo = true;
	}
	else //Not support video
	{
		threadLoadingBitmap->bitmapIcone = application_context.GetDefaultPicture();
		wxString filename = threadLoadingBitmap->filename;

		//wxBitmap bitmap = wxBitmap(defaultPicture);
		CSqlPhotos SqlPhotos;
		int photoId = SqlPhotos.GetPhotoId(filename);
		if (photoId != -1)
		{
			CSqlThumbnailVideo sqlThumbnailVideo;
			wxString localName = sqlThumbnailVideo.InsertThumbnail(photoId, filename, application_context.GetWxDefaultPicture().GetWidth(), application_context.GetWxDefaultPicture().GetHeight(), 0, 0, 0, 0);
			application_context.GetWxDefaultPicture().SaveFile(localName, wxBITMAP_TYPE_JPEG);
		}

	}

	listVideo.clear();

	auto event = new wxCommandEvent(wxEVENT_ICONEUPDATE);
	event->SetClientData(threadLoadingBitmap);
	wxQueueEvent(threadLoadingBitmap->window, event);

}

void CThumbnailVideo::UpdateVideoThumbnail()
{
	if (videoFilename != "")
	{
		CSqlThumbnailVideo sqlThumbnailVideo;
		int nbResult = sqlThumbnailVideo.GetNbThumbnail(videoFilename);
		CSqlPhotos SqlPhotos;
		int photoId = SqlPhotos.GetPhotoId(videoFilename);
		if (nbResult > 0)
		{
			for (int i = 0; i < nbResult; i++)
			{
                CIcone * pBitmapIcone = iconeList->GetElement(i);
 				if (pBitmapIcone != nullptr)
				{
					auto thumbnailData = dynamic_cast<CThumbnailDataStorage*>(pBitmapIcone->GetPtData()); 
                    if (thumbnailData != nullptr) 
                    {
                        auto thumbnail = std::make_unique<CImageVideoThumbnail>();
                        sqlThumbnailVideo.GetPictureThumbnail(photoId, videoFilename, i, thumbnail.get());
                        thumbnail->percent = static_cast<float>(i) / static_cast<float>(nbResult) * 100.0f;
                            
                        if (!thumbnail->image.empty())
                        {
                            thumbnailData->SetIsDefault(false);
                            thumbnailData->SetBitmap(thumbnail->image);
                        }  
                            
                        thumbnailData->SetTimePosition(thumbnail->timePosition);   
                    }                     
                }
			}
		}
	}

	process_end = true;
}

void CThumbnailVideo::UpdateVideoThumbnail(wxCommandEvent& event)
{
	//if (!videoThumbnailOk)
	UpdateVideoThumbnail();
}

void CThumbnailVideo::ResizeThumbnail()
{
	UpdateScroll();
    UpdateVideoThumbnail();
}

void CThumbnailVideo::EraseThumbnail(long value)
{

	if (value == 1)
	{
		CSqlPhotos SqlPhotos;
		int photoId = SqlPhotos.GetPhotoId(videoFilename);

		CSqlThumbnailVideo sqlThumbnailvideo;
		sqlThumbnailvideo.DeleteThumbnail(photoId);

		CSqlThumbnail sqlThumbnail;
		sqlThumbnail.DeleteThumbnail(videoFilename);
	}
	thumbnailPos = 0;


	for (int i = 0; i < nbElementInIconeList; i++)
	{
		wxString filelocalName = iconeList->GetFilename(i);
		if (videoFilename == filelocalName)
		{
			CIcone * pIcone = iconeList->GetElement(i);
			if (pIcone != nullptr)
			{
				CThumbnailDataStorage * pThumbnailData = (CThumbnailDataStorage *)pIcone->GetPtData();
				if (pThumbnailData != nullptr)
				{
					pThumbnailData->InitLoadState();
                    pThumbnailData->SetIsDefault(true);
					wxString filename = pThumbnailData->GetFilename();
					pThumbnailData->SetIsProcess(false);
					pThumbnailData->SetIsLoading(false);
				}
			}
		}
	}


	CLibPicture libPicture;
	int nbImage = libPicture.GetNbImage(videoFilename);
	InitScrollingPos();
	InitWithDefaultPicture(videoFilename, nbImage);
	process_end = false;
	threadDataProcess = true;
	needToRefresh = true;

	wxWindow* window = this->FindWindowById(MAINVIEWERWINDOWID);
	if (window != nullptr)
	{
		wxString* localName = new wxString(videoFilename);
		wxCommandEvent evt(wxEVENT_ICONETHUMBNAILGENERATION);
		evt.SetClientData(localName);
		evt.SetInt(1);
		evt.SetExtraLong(localid);
		window->GetEventHandler()->AddPendingEvent(evt);
	}

	
}

void CThumbnailVideo::EraseThumbnail(wxCommandEvent& event)
{
    long value = event.GetExtraLong();
    EraseThumbnail(value);
}


void CThumbnailVideo::SetFile(const wxString& videoFile, const int& size)
{
	process_end = false;
    
   // printf("CThumbnailVideo::SetFile : %s \n", videoFile.utf8_string().c_str());

	CLibPicture libPicture;
	iFormat = libPicture.TestImageFormat(videoFile);
	videoFilename = videoFile;
	InitScrollingPos();
	InitWithDefaultPicture(videoFile, size);
	
	process_end = false;
	threadDataProcess = true;
	needToRefresh = true;

}
