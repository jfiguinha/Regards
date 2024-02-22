#include <header.h>
#include "ThumbnailViewerPicture.h"
#include "MainWindow.h"
#include <ThumbnailDataSQL.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarWnd.h>
#include "ThumbnailBuffer.h"
#include <SqlFindPhotos.h>
using namespace Regards::Viewer;
using namespace Regards::Sqlite;


std::mutex CThumbnailViewerPicture::localmu;

CThumbnailViewerPicture::CThumbnailViewerPicture(wxWindow* parent, wxWindowID id, const CThemeThumbnail& themeThumbnail,
                                                 const bool& testValidity)
	: CThumbnailVertical(parent, id, themeThumbnail, testValidity)
{
	widthThumbnail = 0;
	heightThumbnail = 0;
	preprocess_thumbnail = true;
}


CThumbnailViewerPicture::~CThumbnailViewerPicture(void)
{
}

void CThumbnailViewerPicture::OnPictureClick(CThumbnailData* data)
{
	auto mainWindow = static_cast<CMainWindow*>(this->FindWindowById(MAINVIEWERWINDOWID));
	if (mainWindow != nullptr)
	{
		wxCommandEvent evt(wxEVENT_ONPICTURECLICK);
		evt.SetExtraLong(data->GetNumPhotoId());
		mainWindow->GetEventHandler()->AddPendingEvent(evt);
	}
}


void CThumbnailViewerPicture::Init(const int& typeAffichage)
{
	SetListeFile();

}

vector<wxString> CThumbnailViewerPicture::GetFileList()
{
	vector<wxString> list;
	CSqlFindPhotos sqlFindPhotos;
	sqlFindPhotos.SearchPhotos(&list);

	return list;
}


CIconeList * CThumbnailViewerPicture::PregenerateList()
{
	auto iconeListLocal = new CIconeList();
	CIconeList* oldIconeList = nullptr;
	int size = CThumbnailBuffer::GetVectorSize();

	//std::map<int, CIcone * >
#ifndef USE_TBB_VECTOR
	for (auto i = 0; i < size; i++)
#else
	tbb::parallel_for(0, size, 1, [=](int i)
#endif
	{
		try
		{
			CPhotos photo = CThumbnailBuffer::GetVectorValue(i);

			std::vector<CIcone*>::iterator it = std::find_if(pIconeList.begin(), pIconeList.end(), [&](CIcone* e)
				{
					CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)e->GetData();
					return thumbnailData->GetNumPhotoId() == photo.GetId();

				});

			if (it == pIconeList.end())
			{
				CPhotos photo = CThumbnailBuffer::GetVectorValue(i);
				wxString filename = photo.GetPath();
				auto thumbnailData = new CThumbnailDataSQL(filename, false, false);
				thumbnailData->SetNumPhotoId(photo.GetId());
				thumbnailData->SetNumElement(i);

				auto pBitmapIcone = new CIcone();
				pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
				pBitmapIcone->SetData(thumbnailData);
				pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
				pBitmapIcone->SetWindowPos(i * themeThumbnail.themeIcone.GetWidth(), 0);
				pBitmapIcone->SetFilename(filename);
				iconeListLocal->AddElement(pBitmapIcone);

				pIconeList.push_back(pBitmapIcone);
			}
			else
			{
				CIcone* icone = (CIcone*)*it;
				CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)icone->GetData();
				thumbnailData->SetNumElement(nbElement++);
				icone->SetNumElement(thumbnailData->GetNumElement());
				icone->SetWindowPos(i * themeThumbnail.themeIcone.GetWidth(), 0);
				iconeListLocal->AddElement(icone);
			}
		}
		catch (...)
		{

		}
	}
#ifdef USE_TBB_VECTOR
	);
#endif

	iconeListLocal->SortByFilename();

#ifndef USE_TBB_VECTOR
	for (auto i = 0; i < size; i++)
#else
	tbb::parallel_for(0, size, 1, [=](int i)
#endif
	{
		CIcone *  icone = iconeListLocal->GetElement(i);
		icone->SetNumElement(i);
		CThumbnailDataSQL* data = (CThumbnailDataSQL*)icone->GetData();
		data->SetNumElement(i);
	}
#ifdef USE_TBB_VECTOR
	);
#endif

	return iconeListLocal;
}


void CThumbnailViewerPicture::ApplyListeFile()
{
	std::vector<CIcone*> pIconeListToClean;
	CIconeList* oldIconeList = iconeList;
	threadDataProcess = false;

	/*
	PhotosVector _pictures;
	CSqlFindPhotos sqlFindPhotos;
	sqlFindPhotos.SearchPhotosByCriteria(&_pictures);
	*/
	CIconeList* iconeListLocal = PregenerateList();

	iconeList = iconeListLocal;


	nbElementInIconeList = iconeList->GetNbElement();
    
    for (CIcone* ico : pIconeList)
	{
		bool find = false;
		CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)ico->GetData();
		int photoId = thumbnailData->GetNumPhotoId();

		for (int i = 0; i < iconeList->GetNbElement(); i++)
		{
			CIcone* ico = iconeList->GetElement(i);
			CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)ico->GetData();
			if (thumbnailData->GetNumPhotoId() == photoId)
			{
				find = true;
				break;
			}
		}

		if (!find)
			pIconeListToClean.push_back(ico);
	}

	//------------------------------------
	for (CIcone* ico : pIconeListToClean)
	{
		CThumbnailDataSQL* _clean = (CThumbnailDataSQL*)ico->GetData();

		std::vector<CIcone*>::iterator it = std::find_if(pIconeList.begin(), pIconeList.end(), [&](CIcone* e)
			{
				CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)e->GetData();
				return thumbnailData->GetNumPhotoId() == _clean->GetNumPhotoId();

			});

		if (it != pIconeList.end())
			pIconeList.erase(it);
	}

	EraseThumbnailList(oldIconeList);
	EraseIconeList(pIconeListToClean);
	//----------------------------------------------------------

	//EraseThumbnailList(oldIconeList);

	oldIconeList = nullptr;

	AfterSetList();

	ResizeThumbnail();

	threadDataProcess = true;

	needToRefresh = true;
}

void CThumbnailViewerPicture::SetListeFile()
{
	std::vector<CIcone*> pIconeListToClean;
	auto iconeListLocal = new CIconeList();
	CIconeList* oldIconeList = nullptr;
	threadDataProcess = false;

	int i = 0;
	int x = 0;
	int y = 0;
	thumbnailPos = 0;
	int size = CThumbnailBuffer::GetVectorSize();

	//std::map<int, CIcone * >
#ifndef USE_TBB_VECTOR
	for (auto i = 0; i < size; i++)
#else
	tbb::parallel_for(0, size, 1, [=](int i)
#endif
	{
		CPhotos photo = CThumbnailBuffer::GetVectorValue(i);

		std::vector<CIcone*>::iterator it = std::find_if(pIconeList.begin(), pIconeList.end(), [&](CIcone* e)
			{
				CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)e->GetData();
				return thumbnailData->GetNumPhotoId() == photo.GetId();

			});

		if (it == pIconeList.end())
		{
			wxString filename = photo.GetPath();
			auto thumbnailData = new CThumbnailDataSQL(filename, false, false);
			thumbnailData->SetNumPhotoId(photo.GetId());
			thumbnailData->SetNumElement(i);

			auto pBitmapIcone = new CIcone();
			pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
			pBitmapIcone->SetData(thumbnailData);
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(i * themeThumbnail.themeIcone.GetWidth(), 0);
			pBitmapIcone->SetFilename(filename);
			iconeListLocal->AddElement(pBitmapIcone);

			pIconeList.push_back(pBitmapIcone);
		}
		else
		{
			CIcone* icone = (CIcone*)*it;
			CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)icone->GetData();
			thumbnailData->SetNumElement(nbElement++);
			icone->SetNumElement(thumbnailData->GetNumElement());
			icone->SetWindowPos(i * themeThumbnail.themeIcone.GetWidth(), 0);
			iconeListLocal->AddElement(icone);
		}
	}
#ifdef USE_TBB_VECTOR
    );
#endif

	iconeListLocal->SortById();

	oldIconeList = iconeList;
	iconeList = iconeListLocal;

	nbElementInIconeList = iconeList->GetNbElement();
    
    for (CIcone* ico : pIconeList)
	{
		bool find = false;
		CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)ico->GetData();
		int photoId = thumbnailData->GetNumPhotoId();

		for (int i = 0; i < iconeList->GetNbElement(); i++)
		{
			CIcone* ico = iconeList->GetElement(i);
			CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)ico->GetData();
			if (thumbnailData->GetNumPhotoId() == photoId)
			{
				find = true;
				break;
			}
		}

		if (!find)
			pIconeListToClean.push_back(ico);
	}

	//------------------------------------
	for (CIcone* ico : pIconeListToClean)
	{
		CThumbnailDataSQL* _clean = (CThumbnailDataSQL*)ico->GetData();

		std::vector<CIcone*>::iterator it = std::find_if(pIconeList.begin(), pIconeList.end(), [&](CIcone* e)
			{
				CThumbnailDataSQL* thumbnailData = (CThumbnailDataSQL*)e->GetData();
				return thumbnailData->GetNumPhotoId() == _clean->GetNumPhotoId();

			});

		if (it != pIconeList.end())
			pIconeList.erase(it);
	}

	EraseThumbnailList(oldIconeList);
	EraseIconeList(pIconeListToClean);
	//----------------------------------------------------------

	//EraseThumbnailList(oldIconeList);

	AfterSetList();

	ResizeThumbnail();

	threadDataProcess = true;

	needToRefresh = true;
}

void CThumbnailViewerPicture::ResizeThumbnail()
{
	ResizeThumbnailWithoutVScroll();

	widthThumbnail = GetWindowWidth();
	heightThumbnail = GetWindowHeight();

	UpdateScroll();
}

void CThumbnailViewerPicture::ResizeThumbnailWithoutVScroll()
{
#ifndef USE_TBB_VECTOR
	for (auto i = 0; i < nbElementInIconeList; i++)
#else
	tbb::parallel_for(0, nbElementInIconeList, 1, [=](int i)
#endif    
	{
		CIcone *  pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(i * themeThumbnail.themeIcone.GetWidth(), 0);
			//x += themeThumbnail.themeIcone.GetWidth();
		}
	}
#ifdef USE_TBB_VECTOR
    );
#endif

}


void CThumbnailViewerPicture::RenderIconeWithoutVScroll(wxDC* deviceContext)
{
	for (auto i = 0; i < nbElementInIconeList; i++)
	{
        try
        {
            CIcone *  pBitmapIcone = iconeList->GetElement(i);
            if (pBitmapIcone != nullptr)
            {
                pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
                wxRect rc = pBitmapIcone->GetPos();
                //if visible
                int left = rc.x - posLargeur;
                int right = rc.x + rc.width - posLargeur;

                if (right >= 0 && left <= GetWindowWidth())
                {
                    RenderBitmap(deviceContext, pBitmapIcone, -posLargeur, 0);
                }
            }
        }
        catch(...)
        {
            
        }
	}

	
}


bool CThumbnailViewerPicture::ItemCompFonct(int xPos, int yPos, CIcone *  icone, CWindowMain* parent) /* Définit une fonction. */
{
	CThumbnailViewerPicture* viewerPicture = (CThumbnailViewerPicture*)parent;
	wxRect rc = icone->GetPos();
	int left = rc.x - viewerPicture->GetLargeur();
	int right = rc.x + rc.width - viewerPicture->GetLargeur();
	int top = rc.y - viewerPicture->GetHauteur();
	int bottom = rc.y + rc.height - viewerPicture->GetHauteur();
	if ((left < xPos && xPos < right) && (top < yPos && yPos < bottom))
	{
		return true;
	}
	return false;
}

CIcone *  CThumbnailViewerPicture::FindElement(const int& xPos, const int& yPos)
{
	pItemCompFonct _pf = &ItemCompFonct;
	CIcone *  icone = iconeList->FindElement(xPos, yPos, &_pf, this);
	return icone;
}
