#include <header.h>
#include "ShowElement.h"
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
#include <MetadataExiv2.h>
#include <DeepLearning.h>

#include "BitmapWndViewer.h"

//--------------------------------------------------
//Video
//--------------------------------------------------
#include "SliderVideo.h"
#include "VideoControl_soft.h"
#include <window_id.h>
#include "SlideToolbar.h"
#include <ViewerParam.h>
#include <BitmapWnd3d.h>
#include <SavePicture.h>
#include <ImageLoadingFormat.h>
using namespace Regards::Video;
using namespace Regards::Window;
using namespace Regards::OpenCL;
using namespace Regards::Viewer;
using namespace Regards::Sqlite;
using namespace Regards::Picture;
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

	~CThreadRotate()
	{
	};

	bool isReady;
	int exif;
	wxString filename;
	cv::Mat bitmap;
	std::thread* thread;
	wxWindow* mainWindow;
};


void CShowElement::SetFullscreen(const bool& fullscreen)
{
	this->fullscreen = fullscreen;
	bitmapWindow->SetFullscreen(fullscreen);
}

void CShowElement::UpdateScreenRatio()
{
	//if (!isVideo)
	//{
	scrollbar->UpdateScreenRatio();
	pictureToolbar->UpdateScreenRatio();
	bitmapWindowRender->UpdateScreenRatio();
	//}
	//else
	//{
	slideToolbar->UpdateScreenRatio();
	videoSlider->UpdateScreenRatio();
	//}
	this->Resize();
}

void CShowElement::ReloadResource()
{
	if (!isVideo)
		bitmapWindow->ReloadResource();
	else
		videoWindow->ReloadResource();
}


CShowElement::CShowElement(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId,
                           wxWindowID mainViewerId, CBitmapInterface* bitmapInterface, CThemeParam* config,
                           const bool& exportPicture)
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
	windowMain = static_cast<CWindowMain*>(this->FindWindowById(mainViewerId));


	CThemeBitmapWindow themeBitmap;
	configRegards = CParamInit::getInstance();
	CThemeScrollBar themeScroll;
	CThemeToolbar themeToolbar;
	std::vector<int> value = {
		1, 2, 3, 4, 5, 6, 8, 12, 16, 25, 33, 50, 66, 75, 100, 133, 150, 166, 200, 300, 400, 500, 600, 700, 800, 1200,
		1600
	};

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

	//			CBitmapWndRender(wxWindow* parent, wxWindowID id, CSliderInterface* slider, wxWindowID idMain, const CThemeBitmapWindow& theme);

	bitmapWindow = new CBitmapWndViewer(pictureToolbar, mainViewerId, themeBitmap, bitmapInterface);
	bitmapWindow->SetTabValue(value);

	bitmapWindowRender = new CBitmapWnd3D(this, bitmapViewerId);
	bitmapWindowRender->SetBitmapRenderInterface(bitmapWindow);

	if (config != nullptr)
		config->GetScrollTheme(&themeScroll);

	scrollbar = nullptr;

	scrollbar = new CScrollbarWnd(this, bitmapWindowRender, wxID_ANY, "BitmapScroll");

	Connect(wxEVT_IDLE, wxIdleEventHandler(CShowElement::OnIdle));
	Connect(wxEVT_BITMAPDBLCLICK, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowElement::OnViewerDblClick));
	Connect(wxEVT_BITMAPZOOMIN, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowElement::OnViewerZoomIn));
	Connect(wxEVT_BITMAPZOOMOUT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowElement::OnViewerZoomOut));
	Connect(wxEVENT_MOVELEFT, wxCommandEventHandler(CShowElement::OnMoveLeft));
	Connect(wxEVENT_MOVERIGHT, wxCommandEventHandler(CShowElement::OnMoveRight));
	Connect(wxEVENT_MOVETOP, wxCommandEventHandler(CShowElement::OnMoveTop));
	Connect(wxEVENT_MOVEBOTTOM, wxCommandEventHandler(CShowElement::OnMoveBottom));
	Connect(wxEVENT_SETCONTROLSIZE, wxCommandEventHandler(CShowElement::OnControlSize));
	Connect(wxEVENT_SETPOSITION, wxCommandEventHandler(CShowElement::OnSetPosition));
	Connect(wxEVENT_ROTATEDETECT, wxCommandEventHandler(CShowElement::OnRotateDetect));

	//--------------------------------------------------------------------
	//Video
	//--------------------------------------------------------------------
	Connect(wxEVENT_SAVE, wxCommandEventHandler(CShowElement::OnSave));
	Connect(wxEVENT_SHRINKPOS, wxCommandEventHandler(CShowElement::OnValueShrinkChange));
	Connect(wxEVENT_ZOOMPOS, wxCommandEventHandler(CShowElement::OnValueChange));
	Connect(wxEVENT_SHRINK, wxCommandEventHandler(CShowElement::OnShrink));
	Connect(wxEVENT_CLOSE, wxCommandEventHandler(CShowElement::OnClose));

	progressValue = 0;
	filename = "";

	softRender = true;
	toolbarOutside = false;
	height = 0;
	width = 0;
	videoPosOld = 0;
	isDiaporama = false;
	//decoder = "";
	CThemeSlider themeSlider;
	CTheme themeVideo;

	if (config != nullptr)
	{
		config->GetVideoControlTheme(&themeVideo);
	}

	softRender = true;
	videoWindow = new CVideoControlSoft(windowMain, this, this);
	bitmapWindowRender->SetBitmapRenderInterface(videoWindow);
	bitmapWindowRender->UpdateRenderInterface(bitmapWindow);

	if (config != nullptr)
	{
		CThemeSlider theme;
		config->GetVideoSliderTheme(&theme);
	}

	if (config != nullptr)
	{
		config->GetBitmapToolbarTheme(&themeToolbar);
	}


	videoSlider = new CSliderVideo(this, wxID_ANY, this, themeSlider);
	slideToolbar = new CSlideToolbar(this, wxID_ANY, themeToolbar);

	for (int i = 0; i < 101; i++)
		sound_value.push_back(i);
	slideToolbar->SetTabValue(sound_value);
	play = false;

	videoWindow->PlayFirstMovie(false);
	videoSlider->Show(false);
	slideToolbar->Show(false);
	scrollbar->Show(true);
	bitmapWindowRender->Show(true);
	isVideo = false;
}

void CShowElement::OnControlSize(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
		evt.SetClientData(event.GetClientData());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowElement::OnSetPosition(wxCommandEvent& event)
{
	if (!isVideo)
	{
		if (scrollbar != nullptr)
		{
			wxCommandEvent evt(wxEVENT_SETPOSITION);
			evt.SetClientData(event.GetClientData());
			scrollbar->GetEventHandler()->AddPendingEvent(evt);
		}
	}
	else
	{
		long pos = event.GetExtraLong();
		SetTimePosition(pos * 1000 * 1000);
	}
}

void CShowElement::OnMoveLeft(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVELEFT);
		evt.SetInt(event.GetInt());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowElement::OnMoveRight(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVERIGHT);
		evt.SetInt(event.GetInt());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowElement::OnMoveTop(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVETOP);
		evt.SetInt(event.GetInt());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowElement::OnMoveBottom(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVEBOTTOM);
		evt.SetInt(event.GetInt());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowElement::OnViewerDblClick(wxCommandEvent& event)
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


void CShowElement::StopTransition()
{
	if (bitmapWindow != nullptr)
		bitmapWindow->StopTransition();
}

void CShowElement::ShowToolbar()
{
	showToolbar = true;
	if (!isVideo)
	{
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
	else
	{
		if (videoSlider != nullptr)
		{
			if (!videoSlider->IsShown())
				videoSlider->Show(true);
			if (!slideToolbar->IsShown() && showToolbarSup)
				slideToolbar->Show(true);
		}
		this->Resize();
	}
}

void CShowElement::Resize()
{
	int width = GetWindowWidth();
	int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;

	if (!isVideo)
	{
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

		if (!showToolbar && pictureToolbar->IsShown())
		{
			HideToolbar();
		}

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
	else
	{
		if (isDiaporama && videoSlider != nullptr && videoWindow != nullptr)
		{
			slideToolbar->SetSize(0, 0, 0, 0);
			slideToolbar->Refresh();
			videoSlider->SetSize(0, 0, 0, 0);
			videoSlider->Refresh();
			scrollbar->SetSize(0, 0, width, height);
			scrollbar->Refresh();
		}
		else if (videoSlider != nullptr)
		{
			//CDeferPos deferpos;
			int posHeight = 0;
			int posHeightSup = 0;

			if (showToolbar)
			{
				if (videoSlider->IsShown())
					posHeight += videoSlider->GetHeight();

				if (showToolbarSup && slideToolbar->IsShown())
					posHeightSup = posHeight;
			}


			int positionHeight = height - posHeight - posHeightSup;

			scrollbar->SetSize(0, 0, width, positionHeight);
			scrollbar->Resize();

			if (showToolbar)
			{
				if (videoSlider->IsShown())
				{
					if (showToolbarSup)
					{
						slideToolbar->SetSize(0, positionHeight, width, videoSlider->GetHeight(), TRUE);
						slideToolbar->Refresh();
					}

					videoSlider->SetSize(0, positionHeight + posHeightSup, width, videoSlider->GetHeight(), TRUE);
					videoSlider->Refresh();
				}
			}

		}
		else
		{
			scrollbar->SetSize(0, 0, width, height);
			scrollbar->Refresh();
		}
	}
}

void CShowElement::SetBitmapPreviewEffect(const int& effect)
{
	if (bitmapWindow != nullptr)
	{
		bitmapWindow->SetBitmapPreviewEffect(effect);
	}
}


void CShowElement::TransitionEnd()
{
	transitionEnd = true;
	if (tempImage != nullptr)
	{
		bitmapWindow->SetBitmap(tempImage);
		tempImage = nullptr;
		transitionEnd = false;
		if (pictureToolbar != nullptr)
			pictureToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());
		//bitmapWindow->Refresh();
	}
}

void CShowElement::OnIdle(wxIdleEvent& evt)
{
	if (refreshSlider && videoSlider->IsShown())
		videoSlider->Refresh();

	refreshSlider = false;
}

//---------------------------------------------------------------------------------------
//Test FacialRecognition
//---------------------------------------------------------------------------------------
void CShowElement::RotateRecognition(void* param)
{
	auto threadRotate = static_cast<CThreadRotate*>(param);
	if (threadRotate != nullptr)
	{
		if (!threadRotate->bitmap.empty())
		{
			bool fastDetection = true;
			CRegardsConfigParam* param = CParamInit::getInstance();
			if (param != nullptr)
				fastDetection = param->GetFastDetectionFace();

			threadRotate->isReady = true;
			//cv::flip(threadRotate->bitmap, threadRotate->bitmap, 0);
			threadRotate->exif = DeepLearning::CDeepLearning::GetExifOrientation(threadRotate->bitmap, fastDetection);
		}

		if (threadRotate->mainWindow != nullptr)
		{
			wxCommandEvent evt(wxEVENT_ROTATEDETECT);
			evt.SetClientData(threadRotate);
			threadRotate->mainWindow->GetEventHandler()->AddPendingEvent(evt);
		}
	}
}

void CShowElement::OnRotateDetect(wxCommandEvent& event)
{
	auto path = static_cast<CThreadRotate*>(event.GetClientData());
	if (path->thread != nullptr)
	{
		path->thread->join();
		delete(path->thread);
		path->thread = nullptr;

		if (path->filename == filename && path->isReady)
		{
			bitmapWindow->SetOrientation(path->exif);
		}


		CSqlPhotos sqlPhotos;
		sqlPhotos.InsertPhotoExif(path->filename, path->exif);
	}

	if (path != nullptr)
		delete path;
}

void CShowElement::IsNextPicture(const bool& value)
{
	bitmapWindow->SetNextPictureMove(value);
}

int CShowElement::GetAngleAndFlipFromExif(const int& exif, int& angle, int& flipH, int& flipV)
{
	switch (exif)
	{
	case 0:
		angle = 0;
		flipH = 0;
		flipV = 0;
		break;
	case 1:
		angle = 0;
		flipH = 1;
		flipV = 0;
		break;
	case 2:
		angle = 0;
		flipH = 0;
		flipV = 1;
		break;
	case 3:
		angle = 0;
		flipH = 1;
		flipV = 1;
		break;
	case 4:
		angle = 90;
		flipH = 0;
		flipV = 0;
		break;
	case 5:
		angle = 90;
		flipH = 1;
		flipV = 0;
		break;
	case 6:
		angle = 90;
		flipH = 0;
		flipV = 1;
		break;
	case 7:
		angle = 90;
		flipH = 1;
		flipV = 1;
		break;
	case 8:
		angle = 180;
		flipH = 0;
		flipV = 0;
		break;
	case 9:
		angle = 180;
		flipH = 1;
		flipV = 0;
		break;
	case 10:
		angle = 180;
		flipH = 0;
		flipV = 1;
		break;
	case 11:
		angle = 180;
		flipH = 1;
		flipV = 1;
		break;
	case 12:
		angle = 270;
		flipH = 0;
		flipV = 0;
		break;
	case 13:
		angle = 270;
		flipH = 1;
		flipV = 0;
		break;
	case 14:
		angle = 270;
		flipH = 0;
		flipV = 1;
		break;
	case 15:
		angle = 270;
		flipH = 1;
		flipV = 1;
		break;
	}


	return 0;
}

bool CShowElement::SetBitmap(CImageLoadingFormat* bitmap, const bool& isThumbnail)
{
	isVideo = false;
	pictureToolbar->Show(true);
	slideToolbar->Show(false);
	videoSlider->Show(false);
	bitmapWindowRender->UpdateRenderInterface(bitmapWindow);

	if (tempImage != nullptr && !isThumbnail)
	{
		delete tempImage;
		tempImage = nullptr;
	}

	if (isThumbnail)
	{
		wxWindow* window = this->FindWindowById(PREVIEWVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_HIDESAVEBUTTON);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
	}
	else
	{
		wxWindow* window = this->FindWindowById(PREVIEWVIEWERID);
		if (window != nullptr)
		{
			wxCommandEvent evt(wxEVENT_SHOWSAVEBUTTON);
			window->GetEventHandler()->AddPendingEvent(evt);
		}
	}
	filename = bitmap->GetFilename();

	if (bitmapWindow != nullptr)
	{
		int angle = 0;
		int flipH = 0;
		int flipV = 0;

		CSqlPhotos sqlPhotos;
		int exif = sqlPhotos.GetPhotoExif(filename);


		if (!isThumbnail && exif > 0)
		{
			CSqlPhotos::GetAngleAndFlip(exif, angle, flipH, flipV);
			//bitmap->SetOrientation(exif);
		}
		int numEffect = 0;
		bool firstPicture = false;
		if (!bitmapWindow->IsPictureLoad())
			firstPicture = true;

		//bitmapWindow->FixArrowNavigation(true);
		bitmapWindow->SetIsBitmapThumbnail(isThumbnail);


		if (isDiaporama)
			numEffect = configRegards->GetDiaporamaTransitionEffect();
		else if (configRegards != nullptr)
			numEffect = configRegards->GetEffect();


		if (numEffect != 0)
		{
			if (firstPicture)
				bitmap->Flip();

			if (isThumbnail || isDiaporama)
			{
				transitionEnd = false;
				bitmap->Flip();
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
					bitmapWindow->ApplyPicturePosition(angle, flipH, flipV);
				}
				else
				{
					transitionEnd = false;
					bitmap->Flip();
					bitmapWindow->ShrinkImage();
					bitmapWindow->SetTransitionBitmap(bitmap);
					tempImage = nullptr;
				}
			}
		}
		else
		{
			bitmapWindow->SetBitmap(bitmap, false);
			bitmapWindow->ApplyPicturePosition(angle, flipH, flipV);
		}

		if (pictureToolbar != nullptr)
			pictureToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());

		this->Resize();
		return true;
	}
	this->Resize();
	return false;
}

CImageLoadingFormat* CShowElement::GetBitmap(const bool& source)
{
	if (bitmapWindow != nullptr)
		return bitmapWindow->GetBitmap(source);

	return nullptr;
}

void CShowElement::OnViewerZoomIn(wxCommandEvent& event)
{
	if (pictureToolbar != nullptr)
		pictureToolbar->ChangeZoomInPos();
}

void CShowElement::OnViewerZoomOut(wxCommandEvent& event)
{
	if (pictureToolbar != nullptr)
		pictureToolbar->ChangeZoomOutPos();
}


void CShowElement::FlipVertical()
{
	if (!isVideo)
	{
		if (bitmapWindow != nullptr)
			bitmapWindow->FlipVertical();
	}
	else
	{
		if (videoWindow != nullptr)
			videoWindow->FlipVertical();
	}
}

void CShowElement::Rotate90()
{
	if (!isVideo)
	{
		if (bitmapWindow != nullptr)
			bitmapWindow->Rotate90();
	}
	else
	{
		if (videoWindow != nullptr)
			videoWindow->Rotate90();
	}
}

void CShowElement::Rotate270()
{
	if (!isVideo)
	{
		if (bitmapWindow != nullptr)
			bitmapWindow->Rotate270();
	}
	else
	{
		if (videoWindow != nullptr)
			videoWindow->Rotate270();
	}
}

void CShowElement::FlipHorizontal()
{
	if (!isVideo)
	{
		if (bitmapWindow != nullptr)
			bitmapWindow->FlipHorizontal();
	}
	else
	{
		if (videoWindow != nullptr)
			videoWindow->FlipHorizontal();
	}
}


void CShowElement::OnValueShrinkChange(wxCommandEvent& event)
{
	slideToolbar->SetTrackBarPosition(videoWindow->GetZoomIndex());
}

bool CShowElement::IsPause()
{
	return videoWindow->IsPause();
}


void CShowElement::OnClose(wxCommandEvent& event)
{
	slideToolbar->Show(false);
	showToolbarSup = false;
	this->Resize();
}

cv::Mat CShowElement::GetVideoBitmap()
{
	cv::Mat bitmap;
	if (videoWindow != nullptr && IsPause())
	{
		bool isFromBuffer = false;
		bitmap = videoWindow->SavePicture(isFromBuffer);
		/*
		if (!isFromBuffer)
		{
			//if (videoWindow->IsFFmpegDecode())
				//cv::flip(bitmap, bitmap, 0);
		}
		*/
	}
	return bitmap;
}

void CShowElement::OnSave(wxCommandEvent& event)
{
	if (videoWindow != nullptr)
	{
		bool isFromBuffer = false;
		cv::Mat bitmap = videoWindow->SavePicture(isFromBuffer);
		auto imageLoading = new CImageLoadingFormat();
		/*
		if (!isFromBuffer)
		{
			if (videoWindow->IsFFmpegDecode())
				cv::flip(bitmap, bitmap, 0);
		}
		*/
		imageLoading->SetPicture(bitmap);
		CSavePicture::SavePicture(nullptr, imageLoading, filename);
		if (imageLoading != nullptr)
			delete imageLoading;
	}
}

void CShowElement::SavePicture()
{
	if (isVideo)
	{
		wxCommandEvent evt(wxEVENT_SAVE);
		this->GetEventHandler()->AddPendingEvent(evt);
	}
	else
	{
		if (bitmapWindow != nullptr)
			bitmapWindow->SavePicture();
	}
}

void CShowElement::OnShrink(wxCommandEvent& event)
{
	videoWindow->ShrinkVideo();
	slideToolbar->SetTrackBarPosition(videoWindow->GetZoomIndex());
	scrollbar->HideHorizontalScroll();
	scrollbar->HideVerticalScroll();
}


void CShowElement::OnValueChange(wxCommandEvent& event)
{
	int value = event.GetInt();
	int mode = slideToolbar->GetMode();
	if (mode == SPEAKER_MODE)
	{
		videoWindow->SetVolume(value);
	}
	else
	{
		scrollbar->ShowHorizontalScroll();
		scrollbar->ShowVerticalScroll();
		videoWindow->SetZoomIndex(value);
	}
}

CVideoControlSoft* CShowElement::GetVideoControl()
{
	return videoWindow;
}

void CShowElement::SetVideoPreviewEffect(CEffectParameter* effectParameter)
{
	if (videoWindow != nullptr)
		videoWindow->SetVideoPreviewEffect(effectParameter);
}

CEffectParameter* CShowElement::GetParameter()
{
	if (videoWindow != nullptr)
		return videoWindow->GetParameter();
	return nullptr;
}

void CShowElement::UpdateFiltre(CEffectParameter* effectParameter)
{
	if (videoWindow != nullptr)
		videoWindow->UpdateFiltre(effectParameter);
}

void CShowElement::ChangeAudio(const int& langue)
{
	videoWindow->ChangeAudioStream(langue);
}

void CShowElement::ClickButton(const int& id)
{
	switch (id)
	{
	case PLAYBUTTONID:
		PlayVideo();
		break;
	case PAUSEBUTTONID:
		PauseVideo();
		break;
	case REPEATID:
		RepeatVideo();
		break;
	case SPEAKERBUTTONID:
		slideToolbar->Show(true);
		showToolbarSup = true;
		slideToolbar->SetSpeakerMode();
		slideToolbar->SetTabValue(sound_value);
		slideToolbar->SetTrackBarPosition(videoWindow->GetVolume());
		this->Resize();
		break;
	case ZOOMBUTTONID:
		slideToolbar->Show(true);
		showToolbarSup = true;
		slideToolbar->SetZoomMode();
		slideToolbar->SetTabValue(videoWindow->GetZoomValue());
		slideToolbar->SetTrackBarPosition(videoWindow->GetZoomIndex());
		this->Resize();
		break;
	case SCREENFORMATID:
		slideToolbar->Show(false);
		showToolbarSup = false;
		videoWindow->ChangeVideoFormat();
		this->Resize();
		break;
	default: ;
	/*
case VOLUMEUPBUTTONID:
	videoWindow->VolumeUp();
	videoSlider->SetVolumePos(videoWindow->GetVolume());
	break;
case VOLUMEDOWNBUTTONID:
	videoWindow->VolumeDown();
	videoSlider->SetVolumePos(videoWindow->GetVolume());
	break;
	*/
	}

	refreshSlider = true;
}

CShowElement::~CShowElement()
{
	delete(scrollbar);
	delete(videoSlider);
	delete(slideToolbar);
	delete(pictureToolbar);
	//delete(videoWindow);
	//delete(bitmapWindowRender);
	delete(bitmapWindow);
}

void CShowElement::SetDiaporamaMode()
{
	isDiaporama = true;

	if (isVideo)
		videoWindow->DiaporamaMode(true);
	else
		bitmapWindow->SetDiaporamaMode();

	this->Resize();
}

void CShowElement::SetNormalMode()
{
	isDiaporama = false;
	if (isVideo)
		videoWindow->DiaporamaMode(false);
	else
		bitmapWindow->SetNormalMode();
	this->Resize();
}

void CShowElement::SetStreamInfo(vector<CStreamInfo>& listAudio, vector<CStreamInfo>& listVideo,
                                 vector<CStreamInfo>& listSubtitle)
{
	listStream.clear();
	for (int i = 0; i < listAudio.size(); i++)
	{
		listStream.push_back(listAudio[i]);
	}

	for (int i = 0; i < listVideo.size(); i++)
	{
		listStream.push_back(listVideo[i]);
	}

	for (int i = 0; i < listSubtitle.size(); i++)
	{
		listStream.push_back(listSubtitle[i]);
	}

	if (windowMain != nullptr)
	{
		wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, wxEVENT_ADDSTREAMINFO);
		evt.SetClientData(&listStream);
		this->windowMain->GetEventHandler()->AddPendingEvent(evt);
	}
}

//------------------------------------------------------------
//Interface Video
//------------------------------------------------------------

void CShowElement::OnVideoEnd()
{
	videoSlider->SetPause();
	if (windowMain != nullptr)
	{
		wxWindow* centralWindow = this->FindWindowById(CENTRALVIEWERWINDOWID);
		if (centralWindow != nullptr)
		{
			wxCommandEvent evt(VIDEO_END_ID);
			centralWindow->GetEventHandler()->AddPendingEvent(evt);
		}
	}
	refreshSlider = true;
}

void CShowElement::OnVideoStop()
{
	videoSlider->SetPause();
	if (windowMain != nullptr)
	{
		wxWindow* centralWindow = this->FindWindowById(CENTRALVIEWERWINDOWID);
		if (centralWindow != nullptr)
		{
			wxCommandEvent evt(wxVIDEO_STOP);
			centralWindow->GetEventHandler()->AddPendingEvent(evt);
		}
	}

	refreshSlider = true;
}


void CShowElement::SetVideoDuration(const int64_t& position)
{
	if (videoWindow != nullptr && videoSlider != nullptr)
	{
		videoTotalTime = position;
		videoSlider->SetTotalSecondTime(position / 1000);
	}
	refreshSlider = true;
}

void CShowElement::OnVideoStart()
{
	if (videoWindow != nullptr && videoSlider != nullptr)
	{
		this->play = true;
		videoSlider->SetPlay();
	}

	wxWindow* centralWindow = this->FindWindowById(CENTRALVIEWERWINDOWID);
	if (centralWindow != nullptr)
	{
		wxCommandEvent evt(VIDEO_START);
		centralWindow->GetEventHandler()->AddPendingEvent(evt);
	}

	refreshSlider = true;
}

void CShowElement::OnVideoPause()
{
}

void CShowElement::OnAfterOpenVideo()
{
	videoSlider->SetTotalSecondTime(0);
	videoSlider->SetPastSecondTime(0);
	ShowSlider(false);
	PlayVideo();
	refreshSlider = true;
}


void CShowElement::OnPositionVideo(const int64_t& position)
{
	if (position >= 0 && position <= videoTotalTime)
	{
		if (videoSlider != nullptr)
			videoSlider->SetPastSecondTime(position);

		int videoPos = position / 1000;
		//if (videoPos != videoPosOld)
		//{
		wxWindow* viewerWindow = this->FindWindowById(CENTRALVIEWERWINDOWID);
		if (viewerWindow != nullptr)
		{
			wxCommandEvent event(VIDEO_UPDATE_ID);
			event.SetExtraLong(videoPos);
			viewerWindow->GetEventHandler()->AddPendingEvent(event);
		}
		videoPosOld = videoPos;

		refreshSlider = true;
	}
}

//------------------------------------------------------------

void CShowElement::SetPosition(const int64_t& timePosition)
{
	if (bitmapWindowRender != nullptr)
	{
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		evt.SetExtraLong(timePosition);
		bitmapWindowRender->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowElement::SetTimePosition(const int64_t& timePosition)
{
	if (videoSlider != nullptr)
	{
		videoSlider->SetPastSecondTime(timePosition / 1000);
		videoSlider->UpdatePositionEvent();
		refreshSlider = true;
	}
}

void CShowElement::MoveSlider(const int64_t& position)
{
	if (bitmapWindowRender != nullptr)
	{
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		evt.SetExtraLong(position);
		bitmapWindowRender->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowElement::InitControl()
{
	if (videoWindow != nullptr)
	{
		Resize();
	}
}

bool CShowElement::SetVideo(const wxString& filename, const int& rotation, const bool& play)
{
	isVideo = true;
	pictureToolbar->Show(false);
	slideToolbar->Show(true);
	videoSlider->Show(true);
	bitmapWindowRender->UpdateRenderInterface(videoWindow);

	videoTotalTime = 0;
	videoPosOld = 0;
	bool value = false;
	this->filename = filename;
	if (videoWindow != nullptr && videoSlider != nullptr)
	{
		value = videoWindow->PlayMovie(filename, true);
	}

	this->Resize();
	return value;
}


void CShowElement::StopVideo(wxString photoName)
{
	play = false;
	ShowSliderToolbar(true);
	videoSlider->SetPastSecondTime(0);
	videoWindow->OnStop(photoName);
	refreshSlider = true;
}

void CShowElement::ShowSliderToolbar(const bool& show)
{
	if (show && !videoSlider->IsShown())
	{
		videoSlider->Show(true);
		Resize();
	}
	else if (!show && videoSlider->IsShown())
	{
		videoSlider->Show(false);
		Resize();
	}
}


void CShowElement::ShowSlider(const bool& show)
{
	if (play)
		ShowSliderToolbar(true);
}


void CShowElement::PlayVideo()
{
	videoWindow->OnPlay();
	videoSlider->SetPlay();
}

void CShowElement::RepeatVideo()
{
	videoWindow->RepeatVideo();
}

void CShowElement::PauseVideo()
{
	play = false;
	videoWindow->OnPause();
	ShowSliderToolbar(true);
	videoSlider->SetPause();
	refreshSlider = true;
}


void CShowElement::HideToolbar()
{
	if (videoSlider != nullptr)
	{
		if (!videoSlider->IsMouseOver())
		{
			if (videoSlider->IsShown())
				videoSlider->Show(false);
			if (slideToolbar->IsShown())
				slideToolbar->Show(false);
		}
	}

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
			}
		}
	}

	this->Resize();
}

bool CShowElement::IsToolbarMouseOver()
{
	if (!isVideo)
	{
		if (videoSlider != nullptr)
			return videoSlider->IsMouseOver();

		return false;
	}
	if (pictureToolbar != nullptr)
		return pictureToolbar->IsMouseOver();

	return false;
}
