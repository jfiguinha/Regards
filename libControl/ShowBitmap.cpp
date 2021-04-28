#include <header.h>
#include "ShowBitmap.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <libPicture.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <array>
#include <ImageLoadingFormat.h>
#include <SqlPhotos.h>
#include <PictureData.h>
#include <MetadataExiv2.h>
#ifdef __APPLE__
#include <DetectFace.h>
#else
#include <DeepLearning.h>
#endif


using namespace Regards::Sqlite;
using namespace Regards::Picture;
using namespace Regards::Window;
using namespace Regards::Control;
using namespace Regards::exiv2;
class CThreadRotate
{
public:

	CThreadRotate()
	{
		isReady = false;
		exif = 0;
		thread = nullptr;
		mainWindow = nullptr;
	};
	~CThreadRotate() {

	};

	bool isReady;
	int exif;
	wxString filename;
	std::thread * thread;
	wxWindow * mainWindow;
};


void CShowBitmap::SetDiaporamaMode()
{
	isDiaporama = true;
	bitmapWindow->SetDiaporamaMode();
	this->Resize();
}

void CShowBitmap::SetNormalMode()
{
	isDiaporama = false;
	bitmapWindow->SetNormalMode();
	this->Resize();
}

void CShowBitmap::SetFullscreen(const bool &fullscreen)
{
	this->fullscreen = fullscreen;
	bitmapWindow->SetFullscreen(fullscreen);
}

void CShowBitmap::UpdateScreenRatio()
{
    scrollbar->UpdateScreenRatio();
    pictureToolbar->UpdateScreenRatio();
    bitmapWindow->UpdateScreenRatio();
    this->Resize();
}

void CShowBitmap::SavePicture()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->SavePicture();
}

CShowBitmap::CShowBitmap(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId,
	wxWindowID mainViewerId, CBitmapInterface * bitmapInterface, CThemeParam * config, const bool &exportPicture)
	: CWindowMain("ShowBitmap", parent, id)
{
	transitionEnd = false;
	tempImage = nullptr;
	scrollbar = nullptr;
	pictureToolbar = nullptr;
	bitmapWindow = nullptr;
	configRegards = nullptr;
	defaultToolbar = true;
	defaultViewer = true;
	isDiaporama = false;
	fullscreen = false;
	showToolbar = true;

	CThemeBitmapWindow themeBitmap;
	configRegards = CParamInit::getInstance();
	CThemeScrollBar themeScroll;
	CThemeToolbar themeToolbar;
	std::vector<int> value = { 1, 2, 3, 4, 5, 6, 8, 12, 16, 25, 33, 50, 66, 75, 100, 133, 150, 166, 200, 300, 400, 500, 600, 700, 800, 1200, 1600 };

	this->bitmapInterface = bitmapInterface;

	if (config != nullptr)
	{
		config->GetBitmapToolbarTheme(&themeToolbar);
	}
    
    pictureToolbar = nullptr;

	pictureToolbar = new CBitmapToolbar(this, wxID_ANY, bitmapViewerId, themeToolbar, false, exportPicture);
	pictureToolbar->SetTabValue(value);

	if (config != nullptr)
		config->GetBitmapWindowTheme(&themeBitmap);

	bitmapWindow = new CBitmapWndViewer(this, bitmapViewerId, pictureToolbar, mainViewerId, themeBitmap, bitmapInterface);

	if (config != nullptr)
		config->GetScrollTheme(&themeScroll);
        
    scrollbar = nullptr;

	scrollbar = new CScrollbarWnd(this, bitmapWindow, wxID_ANY, "BitmapScroll");

	Connect(wxEVT_IDLE, wxIdleEventHandler(CShowBitmap::OnIdle));
    Connect(wxEVT_BITMAPDBLCLICK, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowBitmap::OnViewerDblClick));
    Connect(wxEVT_BITMAPZOOMIN, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowBitmap::OnViewerZoomIn));
    Connect(wxEVT_BITMAPZOOMOUT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowBitmap::OnViewerZoomOut));

	Connect(wxEVENT_MOVELEFT, wxCommandEventHandler(CShowBitmap::OnMoveLeft));
	Connect(wxEVENT_MOVERIGHT, wxCommandEventHandler(CShowBitmap::OnMoveRight));
	Connect(wxEVENT_MOVETOP, wxCommandEventHandler(CShowBitmap::OnMoveTop));
	Connect(wxEVENT_MOVEBOTTOM, wxCommandEventHandler(CShowBitmap::OnMoveBottom));
	Connect(wxEVENT_SETCONTROLSIZE, wxCommandEventHandler(CShowBitmap::OnControlSize));
	Connect(wxEVENT_SETPOSITION, wxCommandEventHandler(CShowBitmap::OnSetPosition));
	Connect(wxEVENT_ROTATEDETECT, wxCommandEventHandler(CShowBitmap::OnRotateDetect));
    progressValue = 0;
	filename = "";
}

void CShowBitmap::OnControlSize(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
		evt.SetClientData(event.GetClientData());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowBitmap::OnSetPosition(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		evt.SetClientData(event.GetClientData());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowBitmap::OnMoveLeft(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVELEFT);
		evt.SetInt(event.GetInt());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowBitmap::OnMoveRight(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVERIGHT);
		evt.SetInt(event.GetInt());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowBitmap::OnMoveTop(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVETOP);
		evt.SetInt(event.GetInt());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowBitmap::OnMoveBottom(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVEBOTTOM);
		evt.SetInt(event.GetInt());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowBitmap::OnViewerDblClick(wxCommandEvent& event)
{
    if (pictureToolbar != nullptr)
    {
        if (pictureToolbar->IsShown())
        {
            HideToolbar();
        }
        else
        {
            ShowToolbar();
        }
    }
}

CShowBitmap::~CShowBitmap()
{

	delete(pictureToolbar);
	delete(bitmapWindow);
	delete(scrollbar);
}

void CShowBitmap::StopTransition()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->StopTransition();
}

void CShowBitmap::HideToolbar()
{
	showToolbar = false;
	if (pictureToolbar != nullptr)
	{
		if (pictureToolbar->IsShown())
		{
			if (!pictureToolbar->IsMouseOver())
			{
				if (bitmapWindow != nullptr)
					bitmapWindow->FixArrowNavigation(false);

				pictureToolbar->Show(false);
				this->Resize();
			}
		}
	}
}

bool CShowBitmap::IsToolbarMouseOver()
{
	if (pictureToolbar != nullptr)
		return pictureToolbar->IsMouseOver();

	return false;
}

void CShowBitmap::ShowToolbar()
{
	showToolbar = true;
	if (pictureToolbar != nullptr)
	{
		if (!pictureToolbar->IsShown())
		{
			if (bitmapWindow != nullptr)
				bitmapWindow->FixArrowNavigation(true);

			pictureToolbar->Show(true);
			this->Resize();
		}
	}
}

void CShowBitmap::Resize()
{

	int width = GetWindowWidth();
	int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;    

	if (!showToolbar && fullscreen)
	{
		scrollbar->HideHorizontalScroll();
		scrollbar->HideVerticalScroll();
	}
	else
	{
		scrollbar->ShowVerticalScroll();
		scrollbar->ShowHorizontalScroll();
	}

	/*
	if (isDiaporama)
	{
		pictureToolbar->SetSize(0, 0, 0, 0);
		scrollbar->SetSize(0, 0, width, height);
	}
	*/
	if (pictureToolbar != nullptr && scrollbar != nullptr)
	{
		if (pictureToolbar->IsShown())
		{
            int pictureWidth = width;
            int pictureHeight = height - pictureToolbar->GetHeight();

            scrollbar->SetSize(0, 0, pictureWidth, pictureHeight);
			scrollbar->Refresh();
            pictureToolbar->SetSize(0, height - pictureToolbar->GetHeight(), width, pictureToolbar->GetHeight());
			pictureToolbar->Refresh();
		}
		else
		{
			scrollbar->SetSize(0, 0, width, height);
			scrollbar->Refresh();
		}
	}
 
	
}

void CShowBitmap::SetBitmapPreviewEffect(const int &effect)
{
	if (bitmapWindow != nullptr)
	{
		bitmapWindow->SetBitmapPreviewEffect(effect);
	}
}


void CShowBitmap::TransitionEnd()
{
	transitionEnd = true;
	if(tempImage != nullptr)
	{
		bitmapWindow->SetBitmap(tempImage);
		tempImage = nullptr;
		transitionEnd = false;
		if (pictureToolbar != nullptr)
			pictureToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());
		bitmapWindow->Refresh();
	}
}

void CShowBitmap::OnIdle(wxIdleEvent& evt)
{
    //TRACE();
	int numEffect = 0;
		
	if (configRegards != nullptr)
		numEffect = configRegards->GetEffect();

	if (numEffect != 0 && transitionEnd)
	{
		if(tempImage != nullptr)
		{
			bitmapWindow->SetBitmap(tempImage);
			tempImage = nullptr;
			transitionEnd = false;
			if (pictureToolbar != nullptr)
				pictureToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());
			bitmapWindow->Refresh();
		}
	}
}

//---------------------------------------------------------------------------------------
//Test FacialRecognition
//---------------------------------------------------------------------------------------
void CShowBitmap::RotateRecognition(void * param)
{
	CThreadRotate * threadRotate = (CThreadRotate *)param;
	if (threadRotate != nullptr)
	{
#ifdef __APPLE__
		bool pictureOK;
		CLibPicture libPicture;
        CImageLoadingFormat * picture = libPicture.LoadPicture(threadRotate->filename);
        if(picture != nullptr)
        {
            CRegardsBitmap * bitmap = picture->GetRegardsBitmap();
            if(bitmap != nullptr)
            {
                CDetectFace detectFace;
                threadRotate->isReady = true;
                threadRotate->exif = detectFace.GetExifOrientation(bitmap); 
                delete bitmap; 
            }

        }
        delete picture;

#else        
		bool pictureOK;
		CLibPicture libPicture;
		CPictureData * pictureData = libPicture.LoadPictureData(threadRotate->filename, pictureOK);
		if (pictureData != nullptr)
		{
			if (pictureOK)
			{
				threadRotate->isReady = true;
				threadRotate->exif = Regards::DeepLearning::CDeepLearning::GetExifOrientation(pictureData);

			}
			delete pictureData;
		}
#endif
		if (threadRotate->mainWindow != nullptr)
		{
			wxCommandEvent evt(wxEVENT_ROTATEDETECT);
			evt.SetClientData(threadRotate);
			threadRotate->mainWindow->GetEventHandler()->AddPendingEvent(evt);
		}
	}


}

void CShowBitmap::OnRotateDetect(wxCommandEvent& event)
{
	CThreadRotate * path = (CThreadRotate *)event.GetClientData();
	if (path->thread != nullptr)
	{
		path->thread->join();
		delete(path->thread);
		path->thread = nullptr;

		if(path->filename == filename && path->isReady)
			bitmapWindow->SetOrientation(path->exif);
	}

	if (path != nullptr)
		delete path;

}

bool CShowBitmap::SetBitmap(CImageLoadingFormat * bitmap, const bool & isThumbnail)
{
	TRACE();
	if (bitmapWindow != nullptr)
	{
		CMetadataExiv2 metaData(bitmap->GetFilename());
		if (configRegards->GetDetectOrientation() && !isThumbnail)
		{
			if (metaData.GetOrientation() == -1)
			{
				CSqlPhotos sqlPhotos;
				int exif = sqlPhotos.GetPhotoExif(bitmap->GetFilename());
				if (exif == -1)
				{
					bool pictureOK;
					CLibPicture libPicture;
					CPictureData* pictureData = libPicture.LoadPictureData(filename, pictureOK);
					if (pictureData != nullptr)
					{
						if (pictureOK)
						{
							bitmap->SetOrientation(Regards::DeepLearning::CDeepLearning::GetExifOrientation(pictureData));
							sqlPhotos.InsertPhotoExif(bitmap->GetFilename(), exif);
						}
						delete pictureData;
					}
				}

				/*
				CThreadRotate* path = new CThreadRotate();
				path->filename = bitmap->GetFilename();
				path->mainWindow = this;
				path->thread = new thread(RotateRecognition, path);
				*/
			}
		}
		filename = bitmap->GetFilename();
		//bitmapWindow->FixArrowNavigation(true);
        bitmapWindow->SetIsBitmapThumbnail(isThumbnail);
		int numEffect = 0;

		if (isDiaporama)
			numEffect = configRegards->GetDiaporamaTransitionEffect();
		else if (configRegards != nullptr)
			numEffect = configRegards->GetEffect();

		if (numEffect != 0)
		{
			if (isThumbnail)
			{
				transitionEnd = false;
				bitmapWindow->ShrinkImage();
				bitmapWindow->SetTransitionBitmap(bitmap);
				tempImage = nullptr;
			}
			else if (isDiaporama)
			{
				transitionEnd = false;
				bitmapWindow->ShrinkImage();
				bitmapWindow->SetTransitionBitmap(bitmap);
				tempImage = nullptr;
			}
			else
			{
				if (transitionEnd)
				{
					bitmapWindow->StopTransition();
					bitmapWindow->SetBitmap(bitmap, false);
				}
				else
					tempImage = bitmap;
			}
		}
		else
			bitmapWindow->SetBitmap(bitmap, false);

		if (pictureToolbar != nullptr)
			pictureToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());
		//bitmapWindow->Refresh();
        
       // wxString filename = bitmap->GetFilename();
        CLibPicture libPicture;
        if(libPicture.GetNbImage(filename) > 1)
        {
            if (pictureToolbar != nullptr)
            {
                pictureToolbar->ShowExportButton();
				wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
				if (window != nullptr)
				{
					wxCommandEvent evt(wxEVENT_HIDESAVEBUTTON);
					window->GetEventHandler()->AddPendingEvent(evt);
				}
            }
			
        }
        else
        {
            if (pictureToolbar != nullptr)
            {
				wxWindow * window = this->FindWindowById(PREVIEWVIEWERID);
				if (window != nullptr)
				{
					wxCommandEvent evt(wxEVENT_SHOWSAVEBUTTON);
					window->GetEventHandler()->AddPendingEvent(evt);
				}
                pictureToolbar->HideExportButton();
            } 
        }
       
		return true;
	}
	return false;
}

CRegardsBitmap * CShowBitmap::GetBitmap(const bool &source)
{
	if (bitmapWindow != nullptr)
		return bitmapWindow->GetBitmap(source);

	return nullptr;
}

void CShowBitmap::OnViewerZoomIn(wxCommandEvent& event)
{
    if(pictureToolbar != nullptr)
        pictureToolbar->ChangeZoomInPos();
}

void CShowBitmap::OnViewerZoomOut(wxCommandEvent& event)
{
    if(pictureToolbar != nullptr)
        pictureToolbar->ChangeZoomOutPos();
}

void CShowBitmap::FlipVertical()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->FlipVertical();
}

void CShowBitmap::Rotate90()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->Rotate90();
}

void CShowBitmap::Rotate270()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->Rotate270();
}

void CShowBitmap::FlipHorizontal()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->FlipHorizontal();
}
