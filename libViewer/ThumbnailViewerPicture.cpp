#include <header.h>
#include "ThumbnailViewerPicture.h"
#include "MainWindow.h"
#include <ThumbnailDataSQL.h>
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

void CThumbnailViewerPicture::PregenerateList()
{
	//iconeList->EraseThumbnailListWithIcon();
	
	int iconWidth = themeThumbnail.themeIcone.GetWidth();
	int size = iconeList->GetNbElement();
	tbb::parallel_for(0, size, 1, [=](int i)
		{
			CIcone* ico = iconeList->GetElement(i);
			bool find = CThumbnailBuffer::FindValidFile(ico->GetFilename());
			if (!find)
				iconeList->RemoveElement(i);
		});

	CIconeList* newIconeList = new CIconeList();
	tbb::parallel_for(0, size, 1, [=](int i)
		{
			CIcone* ico = iconeList->GetElement(i);
			if (ico != nullptr)
			{
				ico->SetNumElement(i);
				newIconeList->AddElement(ico);
			}

		});

	delete iconeList;

	iconeList = newIconeList;

	size = CThumbnailBuffer::GetVectorSize();

	tbb::parallel_for(0, size, 1, [=](int i)
		{
			try
			{
				CPhotos photo = CThumbnailBuffer::GetVectorValue(i);

				wxString filename = photo.GetPath();

				bool find = iconeList->FindElement(photo.GetPath());
				if (!find)
				{
					auto thumbnailData = new CThumbnailDataSQL(filename, false, false);
					thumbnailData->SetNumPhotoId(photo.GetId());
					thumbnailData->SetNumElement(i);

					auto pBitmapIcone = new CIcone();
					pBitmapIcone->SetNumElement(i);
					pBitmapIcone->SetData(thumbnailData);
					pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
					pBitmapIcone->SetWindowPos(i * iconWidth, 0);
					pBitmapIcone->SetFilename(filename);
					iconeList->AddElement(pBitmapIcone);
				}

			}
			catch (const std::exception& e)
			{
				std::cerr << "Error creating icon at index " << i << ": " << e.what() << std::endl;
			}
		});

	iconeList->SortByFilename();

	tbb::parallel_for(0, size, 1, [=](int i)
		{
			CIcone* icone = iconeList->GetElement(i);
			if (icone != nullptr)
			{
				icone->SetNumElement(i);
				auto data = static_cast<CThumbnailDataSQL*>(icone->GetData());
				if (data != nullptr)
				{
					data->SetNumElement(i);
				}
			}
		});


}


void CThumbnailViewerPicture::ApplyListeFile()
{
	threadDataProcess = false;
	PregenerateList();
	nbElementInIconeList = iconeList->GetNbElement();
	AfterSetList();
	ResizeThumbnail();
	threadDataProcess = true;
	needToRefresh = true;
}

void CThumbnailViewerPicture::SetListeFile()
{
	threadDataProcess = false;
	//iconeList->EraseThumbnailListWithIcon();
	int iconWidth = themeThumbnail.themeIcone.GetWidth();

	int size = iconeList->GetNbElement();
	tbb::parallel_for(0, size, 1, [=](int i)
		{
			CIcone* ico = iconeList->GetElement(i);
			bool find = CThumbnailBuffer::FindValidFile(ico->GetFilename());
			if (!find)
				iconeList->RemoveElement(i);
		});

	CIconeList* newIconeList = new CIconeList();
	tbb::parallel_for(0, size, 1, [=](int i)
		{
			CIcone* ico = iconeList->GetElement(i);
			if (ico != nullptr)
			{
				ico->SetNumElement(i);
				newIconeList->AddElement(ico);
			}

		});

	delete iconeList;

	iconeList = newIconeList;

	size = CThumbnailBuffer::GetVectorSize();

	tbb::parallel_for(0, size, 1, [=](int i)
		{
			try
			{
				CPhotos photo = CThumbnailBuffer::GetVectorValue(i);

				wxString filename = photo.GetPath();

				bool find = iconeList->FindElement(photo.GetPath());
				if (!find)
				{
					auto thumbnailData = new CThumbnailDataSQL(filename, false, false);
					thumbnailData->SetNumPhotoId(photo.GetId());
					thumbnailData->SetNumElement(i);

					auto pBitmapIcone = new CIcone();
					pBitmapIcone->SetNumElement(i);
					pBitmapIcone->SetData(thumbnailData);
					pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
					pBitmapIcone->SetWindowPos(i * iconWidth, 0);
					pBitmapIcone->SetFilename(filename);
					iconeList->AddElement(pBitmapIcone);
				}

			}
			catch (const std::exception& e)
			{
				std::cerr << "Error creating icon at index " << i << ": " << e.what() << std::endl;
			}
		});





	iconeList->SortById();

	nbElementInIconeList = iconeList->GetNbElement();


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
	int iconWidth = themeThumbnail.themeIcone.GetWidth();
#ifndef USE_TBB_VECTOR
	for (int i = 0; i < nbElementInIconeList; i++)
#else
	tbb::parallel_for(0, nbElementInIconeList, 1, [this, iconWidth](int i)
#endif    
	{
		CIcone* pBitmapIcone = iconeList->GetElement(i);
		if (pBitmapIcone != nullptr)
		{
			pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
			pBitmapIcone->SetWindowPos(i * iconWidth, 0);
		}
	}
#ifdef USE_TBB_VECTOR
	);
#endif
}


void CThumbnailViewerPicture::RenderIconeWithoutVScroll(wxDC* deviceContext)
{
	for (int i = 0; i < nbElementInIconeList; i++)
	{
		try
		{
			CIcone* pBitmapIcone = iconeList->GetElement(i);
			if (pBitmapIcone != nullptr)
			{
				pBitmapIcone->SetTheme(themeThumbnail.themeIcone);
				const wxRect rc = pBitmapIcone->GetPos();
				
				const int left = rc.x - posLargeur;
				const int right = rc.x + rc.width - posLargeur;

				if (right >= 0 && left <= GetWindowWidth())
				{
					RenderBitmap(deviceContext, pBitmapIcone, -posLargeur, 0);
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error rendering icon at index " << i << ": " << e.what() << std::endl;
		}
	}
}


bool CThumbnailViewerPicture::ItemCompFonct(int xPos, int yPos, CIcone* icone, CWindowMain* parent)
{
	auto viewerPicture = static_cast<CThumbnailViewerPicture*>(parent);
	const wxRect rc = icone->GetPos();
	
	const int left = rc.x - viewerPicture->GetLargeur();
	const int right = rc.x + rc.width - viewerPicture->GetLargeur();
	const int top = rc.y - viewerPicture->GetHauteur();
	const int bottom = rc.y + rc.height - viewerPicture->GetHauteur();
	
	return (left < xPos && xPos < right) && (top < yPos && yPos < bottom);
}

CIcone* CThumbnailViewerPicture::FindElement(const int& xPos, const int& yPos)
{
	pItemCompFonct _pf = &ItemCompFonct;
	return iconeList->FindElement(xPos, yPos, &_pf, this);
}
