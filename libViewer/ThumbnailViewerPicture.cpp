#include <header.h>
#include "ThumbnailViewerPicture.h"
#include "MainWindow.h"
#include <ThumbnailDataSQL.h>
#include <ScrollbarHorizontalWnd.h>
#include <ScrollbarWnd.h>
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
	processIdle = true;
}

vector<wxString> CThumbnailViewerPicture::GetFileList()
{
	/*/
	PhotosVector pictures;
	CSqlFindPhotos sqlFindPhotos;
	sqlFindPhotos.SearchPhotos(&pictures);

	vector<wxString> list;
	for (CPhotos photo : pictures)
	{
		list.push_back(photo.GetPath());
	}
	*/
	vector<wxString> list;
	CSqlFindPhotos sqlFindPhotos;
	sqlFindPhotos.SearchPhotos(&list);

	return list;
}



void CThumbnailViewerPicture::SetListeFile()
{
	PhotosVector pictures;
	auto iconeListLocal = new CIconeList();
	CIconeList* oldIconeList = nullptr;
	threadDataProcess = false;
	CSqlFindPhotos sqlFindPhotos;
	sqlFindPhotos.SearchPhotos(&pictures);
	int i = 0;
	int x = 0;
	int y = 0;
	thumbnailPos = 0;
	int size = pictures.size();

	//std::map<int, CIcone *>
#ifdef __WXGTK__
	for (auto i = 0; i < size; i++)
#else
	tbb::parallel_for(0, size, 1, [=](int i)
#endif
	{
	
		CPhotos fileEntry = pictures[i];
		wxString filename = fileEntry.GetPath();
		auto thumbnailData = new CThumbnailDataSQL(filename, testValidity);
		thumbnailData->SetNumPhotoId(fileEntry.GetId());
		thumbnailData->SetNumElement(i);

		auto pBitmapIcone = new CIcone();
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetWindowPos(i * themeThumbnail.themeIcone.GetWidth(), y);

		iconeListLocal->AddElement(pBitmapIcone);
	}
#ifndef __WXGTK__
    );
#endif

	iconeListLocal->SortById();

	/*
	for (CPhotos fileEntry : pictures)
	{
		wxString filename = fileEntry.GetPath();
		auto thumbnailData = new CThumbnailDataSQL(filename, testValidity);
		thumbnailData->SetNumPhotoId(fileEntry.GetId());
		thumbnailData->SetNumElement(i);


		auto pBitmapIcone = new CIcone();
		pBitmapIcone->SetNumElement(thumbnailData->GetNumElement());
		pBitmapIcone->SetData(thumbnailData);
		pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
		pBitmapIcone->SetWindowPos(x, y);

		iconeListLocal->AddElement(pBitmapIcone);

		x += themeThumbnail.themeIcone.GetWidth();
		i++;
	}*/


	lockIconeList.lock();
	oldIconeList = iconeList;
	iconeList = iconeListLocal;
	lockIconeList.unlock();

	nbElementInIconeList = iconeList->GetNbElement();

	EraseThumbnailList(oldIconeList);

	AfterSetList();

	ResizeThumbnail();

	threadDataProcess = true;

	processIdle = true;

	pictures.clear();
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
#ifdef __WXGTK__
	for (auto i = 0; i < nbElementInIconeList; i++)
#else
	tbb::parallel_for(0, nbElementInIconeList, 1, [=](int i)
#endif    
	{
		CIcone* pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(i * themeThumbnail.themeIcone.GetWidth(), 0);
			//x += themeThumbnail.themeIcone.GetWidth();
		}
	}
#ifndef __WXGTK__    
    );
#endif

	/*
	int x = 0;
	int y = 0;

	for (int i = 0; i < nbElementInIconeList; i++)
	{
		CIcone* pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(x, y);
			x += themeThumbnail.themeIcone.GetWidth();
		}
	}
	*/
}


void CThumbnailViewerPicture::RenderIconeWithoutVScroll(wxDC* deviceContext)
{
	/*
	int numStart = 0;
	for (int i = 0; i < nbElementInIconeList; i++)
	{
		CIcone* pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			wxRect rc = pBitmapIcone->GetPos();
			//if visible
			int left = rc.x - posLargeur;
			int right = rc.x + rc.width - posLargeur;

			// printf("void CThumbnailViewerPicture::RenderIconeWithoutVScroll(wxDC * deviceContext) left : %d right : %d \n", left, right);
			if (right > 0)
			{
				numStart = i;
				break;
			}
		}
	}
	*/

	//for (int i = 0; i < nbElementInIconeList; i++)

	
#ifdef __WXGTK__
	for (auto i = 0; i < nbElementInIconeList; i++)
#else
	tbb::parallel_for(0, nbElementInIconeList, 1, [=](int i)
#endif 
	{
		CIcone* pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			wxRect rc = pBitmapIcone->GetPos();
			//if visible
			int left = rc.x - posLargeur;
			int right = rc.x + rc.width - posLargeur;

			if (right >= 0 && left <= GetWindowWidth())
			{
				localmu.lock();
				RenderBitmap(deviceContext, pBitmapIcone, -posLargeur, 0);
				localmu.unlock();
			}
		}
	}
#ifndef __WXGTK__    
    );
#endif
	
}


bool CThumbnailViewerPicture::ItemCompFonct(int xPos, int yPos, CIcone* icone, CWindowMain* parent) /* Définit une fonction. */
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

CIcone* CThumbnailViewerPicture::FindElement(const int& xPos, const int& yPos)
{
	/*
	for (int i = 0; i < nbElementInIconeList; i++)
	{
		CIcone* icone = iconeList->GetElement(i);
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
	*/

	pItemCompFonct _pf = &ItemCompFonct;
	CIcone* icone = iconeList->FindElement(xPos, yPos, &_pf, this);
	return icone;
}
