#include <header.h>
#include "ShowPreview.h"
#if defined(__WXMSW__)
#include "../include/window_id.h"
#else
#include <window_id.h>
#endif
#include <wx/filename.h>
#include <libPicture.h>
#include <ParamInit.h>
#include <RegardsConfigParam.h>
#include <array>
#include <BitmapWnd3d.h>
#include <BitmapWndRender.h>
#include <ImageLoadingFormat.h>
#include <MetadataExiv2.h>
#include <videothumb.h>
#include <FileUtility.h>
#include <picture_utility.h>
#include <VideoControl_soft.h>
#include "CompressionAudioVideoOption.h"
#include "ffmpeg_transcoding.h"
using namespace Regards::Picture;
using namespace Regards::Window;
using namespace Regards::Control;
using namespace Regards::exiv2;


void CShowPreview::UpdateScreenRatio()
{
	scrollbar->UpdateScreenRatio();
	previewToolbar->UpdateScreenRatio();
	bitmapWindow->UpdateScreenRatio();
	this->Resize();
}

CShowPreview::CShowPreview(wxWindow* parent, wxWindowID id, CThemeParam* config)
	: CWindowMain("ShowBitmap", parent, id)
{
	transitionEnd = false;
	scrollbar = nullptr;
	previewToolbar = nullptr;
	bitmapWindow = nullptr;
	configRegards = nullptr;
	defaultToolbar = true;
	defaultViewer = true;

	CThemeBitmapWindow themeBitmap;
	configRegards = CParamInit::getInstance();
	CThemeScrollBar themeScroll;
	CThemeToolbar themeToolbar;
	CThemeSlider themeSlider;
	std::vector<int> value = {
		1, 2, 3, 4, 5, 6, 8, 12, 16, 25, 33, 50, 66, 75, 100, 133, 150, 166, 200, 300, 400, 500, 600, 700, 800, 1200,
		1600
	};


	if (config != nullptr)
	{
		config->GetBitmapToolbarTheme(&themeToolbar);
	}

	previewToolbar = nullptr;

	previewToolbar = new CPreviewToolbar(this, wxID_ANY, BITMAPWINDOWVIEWERIDDLG, themeToolbar, false);
	previewToolbar->SetTabValue(value);

	if (config != nullptr)
		config->GetBitmapWindowTheme(&themeBitmap);

	themeBitmap.colorScreen = wxColour("black");

	bitmapWindow = new CBitmapWndRender(previewToolbar, 0, themeBitmap);
	bitmapWindowRender = new CBitmapWnd3D(this, BITMAPWINDOWVIEWERIDDLG);
	bitmapWindowRender->SetBitmapRenderInterface(bitmapWindow);
	bitmapWindow->SetTabValue(value);
	bitmapWindow->SetPreview(1);
	if (config != nullptr)
		config->GetScrollTheme(&themeScroll);

	scrollbar = new CScrollbarWnd(this, bitmapWindowRender, wxID_ANY, "BitmapScroll");

	if (config != nullptr)
	{
		config->GetVideoSliderTheme(&themeSlider);
	}

	sliderVideo = new CSliderVideoPreview(this, wxID_ANY, this, themeSlider);

	Connect(wxEVT_BITMAPZOOMIN, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowPreview::OnViewerZoomIn));
	Connect(wxEVT_BITMAPZOOMOUT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(CShowPreview::OnViewerZoomOut));
	Connect(wxEVENT_MOVELEFT, wxCommandEventHandler(CShowPreview::OnMoveLeft));
	Connect(wxEVENT_MOVERIGHT, wxCommandEventHandler(CShowPreview::OnMoveRight));
	Connect(wxEVENT_MOVETOP, wxCommandEventHandler(CShowPreview::OnMoveTop));
	Connect(wxEVENT_MOVEBOTTOM, wxCommandEventHandler(CShowPreview::OnMoveBottom));
	Connect(wxEVENT_SETCONTROLSIZE, wxCommandEventHandler(CShowPreview::OnControlSize));
	Connect(wxEVENT_SETPOSITION, wxCommandEventHandler(CShowPreview::OnSetPosition));

	Connect(wxEVENT_SHOWORIGINAL, wxCommandEventHandler(CShowPreview::OnShowOriginal));
	Connect(wxEVENT_SHOWNEW, wxCommandEventHandler(CShowPreview::OnShowNew));

	Connect(wxEVENT_UPDATEPICTURE, wxCommandEventHandler(CShowPreview::OnUpdatePicture));

	wxString decoder = "";
	progressValue = 0;

	wxString resourcePath = CFileUtility::GetResourcesFolderPath();
	//videoOriginal


	transcodeFFmpeg = new CFFmpegTranscoding();
}

void CShowPreview::SetParameter(const wxString& videoFilename,
                                CVideoOptionCompress* videoOptionCompress)
{
	isFirstPicture = true;
	wxString decoder = "";
	this->videoOptionCompress = *videoOptionCompress;
	progressValue = 0;
	filename = videoFilename;

	if (videoOriginal != nullptr)
		delete videoOriginal;

	videoOriginal = new CVideoThumb(filename, true);
	timeTotal = videoOriginal->GetMovieDuration();
	orientation = videoOriginal->GetOrientation();
	sliderVideo->SetTotalSecondTime(timeTotal * 1000);

	//transcodeFFmpeg->GetFrameOutput(decodeFrameOriginal);
	//decodeFrameOriginal = videoOriginal->GetVideoFramePos(0, 0, 0);

	MoveSlider(0);
	//this->Resize();
}

void CShowPreview::ShowPicture(cv::Mat& bitmap, const wxString& label)
{
	if (!bitmap.empty())
	{
		auto imageLoadingFormat = new CImageLoadingFormat();
		imageLoadingFormat->SetPicture(bitmap);
		if (isFirstPicture)
			bitmapWindow->SetBitmap(imageLoadingFormat, false);
		else
			bitmapWindow->UpdateBitmap(imageLoadingFormat, false);

		if (isFirstPicture)
			bitmapWindow->ShrinkImage();

		auto dlg = static_cast<CompressionAudioVideoOption*>(this->FindWindowByName("CompressionAudioVideoOption"));
		dlg->ChangeLabelPicture(label);

		isFirstPicture = false;
	}
}

void CShowPreview::ShowOriginal()
{
	//cv::Mat mat = CPictureUtility::ApplyRotationVideo(decodeFrameOriginal, orientation);
	//ShowPicture(mat, "Original Video");
	cv::Mat mat = decodeFrameOriginal;
	if (orientation != 0)
	{
		cv::flip(decodeFrameOriginal, mat, -1);
		//cv::rotate(decodeFrameOriginal, mat, cv::ROTATE_90_COUNTERCLOCKWISE);
		//cv::rotate(mat, mat, cv::ROTATE_90_COUNTERCLOCKWISE);
		//cv::flip(decodeFrame, mat, 1);
	}
	ShowPicture(mat, "Original Video");
	//ShowPicture(decodeFrameOriginal, "Original Video");
}

void CShowPreview::ShowNew()
{
	//cv::Mat mat = CPictureUtility::ApplyRotationVideo(decodeFrame, orientation);
	//ShowPicture(mat, "Export Video");
	cv::Mat mat = decodeFrame;
	if (orientation != 0)
	{
		cv::flip(decodeFrame, mat, -1);
		//cv::rotate(decodeFrame, mat, cv::ROTATE_90_COUNTERCLOCKWISE);
		//cv::rotate(mat, mat, cv::ROTATE_90_COUNTERCLOCKWISE);
		//cv::flip(decodeFrame, mat, 1);
	}
		
	ShowPicture(mat, "Original Video");
}

void CShowPreview::OnShowOriginal(wxCommandEvent& event)
{
	ShowOriginal();
	showOriginal = true;
	oldShowOriginal = showOriginal;
}

void CShowPreview::OnShowNew(wxCommandEvent& event)
{
	ShowNew();
	showOriginal = false;
	oldShowOriginal = false;
}

void CShowPreview::OnUpdatePicture(wxCommandEvent& event)
{
	if (!compressIsOK)
	{
		ShowOriginal();
	}
	else
	{
		if (showOriginal)
			ShowOriginal();
		else
			ShowNew();
	}


	if (firstTime)
	{
		if (previewToolbar != nullptr)
			previewToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());

		firstTime = false;
	}


	sliderVideo->Stop();
}

void CShowPreview::SlidePosChange(const int& position, const wxString& key)
{
	if (key == "Move")
	{
		this->key = key;
		moveSlider = true;
		showOriginal = true;
		this->position = position;
		UpdateBitmap(nullptr, "");
	}
	else
	{
		this->key = key;
		showOriginal = oldShowOriginal;
		moveSlider = false;
		this->position = position;
		UpdateBitmap(nullptr, "");
	}
}

void CShowPreview::MoveSlider(const int64_t& position)
{
	showOriginal = oldShowOriginal;
	moveSlider = false;
	this->position = position;
	UpdateBitmap(nullptr, "");
}

void CShowPreview::ThreadLoading(void* data)
{
	int ret = 0;
	auto showPreview = static_cast<CShowPreview*>(data);

	wxString fileTemp = "";

	if (!showPreview->moveSlider)
	{
		if (showPreview->extension == "")
		{
			wxFileName filename(showPreview->filename);
			showPreview->extension = filename.GetExt();
		}
		fileTemp = CFileUtility::GetTempFile("video_temp." + showPreview->extension, true);
		ret = showPreview->transcodeFFmpeg->EncodeFrame(showPreview->filename, fileTemp, showPreview->position,
		                                                &showPreview->videoOptionCompress);
		showPreview->decodeFrameOriginal = showPreview->transcodeFFmpeg->GetFrameOutputWithOutEffect();

		if (ret == 0)
		{
			CVideoThumb video(fileTemp, false);
			showPreview->decodeFrame = video.GetVideoFramePos(0, 0, 0);
#ifndef WIN32_MFT
			//cv::flip(showPreview->decodeFrame, showPreview->decodeFrame, 0);
#endif
		}
		else
			showPreview->compressIsOK = false;

		wxCommandEvent evt(wxEVENT_UPDATEPICTURE);
		showPreview->GetEventHandler()->AddPendingEvent(evt);
	}
	else
	{
		showPreview->decodeFrameOriginal = showPreview->transcodeFFmpeg->GetFrameOutputWithOutEffect();
		wxCommandEvent evt(wxEVENT_UPDATEPICTURE);
		showPreview->GetEventHandler()->AddPendingEvent(evt);
	}

	if (!showPreview->compressIsOK)
	{
		wxCommandEvent evt(wxEVENT_ERRORCOMPRESSION);
		evt.SetInt(ret);
		showPreview->GetParent()->GetParent()->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowPreview::UpdateBitmap(CVideoOptionCompress* videoOptionCompress, const wxString& extension,
                                const bool& updatePicture)
{
	wxString decoder = "";

	if (videoOptionCompress != nullptr)
	{
		this->videoOptionCompress = *videoOptionCompress;
		this->extension = extension;
	}


	sliderVideo->Start();

	if (threadStart != nullptr)
	{
		threadStart->join();
		delete threadStart;
	}

	threadStart = new thread(ThreadLoading, this);
}

void CShowPreview::OnControlSize(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_SETCONTROLSIZE);
		evt.SetClientData(event.GetClientData());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowPreview::OnSetPosition(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_SETPOSITION);
		evt.SetClientData(event.GetClientData());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowPreview::OnMoveLeft(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVELEFT);
		evt.SetInt(event.GetInt());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowPreview::OnMoveRight(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVERIGHT);
		evt.SetInt(event.GetInt());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowPreview::OnMoveTop(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVETOP);
		evt.SetInt(event.GetInt());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

void CShowPreview::OnMoveBottom(wxCommandEvent& event)
{
	if (scrollbar != nullptr)
	{
		wxCommandEvent evt(wxEVENT_MOVEBOTTOM);
		evt.SetInt(event.GetInt());
		scrollbar->GetEventHandler()->AddPendingEvent(evt);
	}
}

CShowPreview::~CShowPreview()
{
	delete(previewToolbar);
	delete(bitmapWindowRender);
	delete(bitmapWindow);
	delete(scrollbar);
	delete(sliderVideo);

	if (transcodeFFmpeg != nullptr)
		delete transcodeFFmpeg;

	if (threadStart != nullptr)
	{
		threadStart->join();
		delete threadStart;
	}
}

void CShowPreview::Resize()
{
	int width = GetWindowWidth();
	int height = GetWindowHeight();
	if (width <= 0 || height <= 0)
		return;

	scrollbar->ShowVerticalScroll();
	scrollbar->ShowHorizontalScroll();

	int pictureWidth = width;
	int pictureHeight = height - previewToolbar->GetHeight() - sliderVideo->GetHeight();

	scrollbar->SetSize(0, 0, pictureWidth, pictureHeight);
	scrollbar->Refresh();
	previewToolbar->SetSize(0, pictureHeight, width, previewToolbar->GetHeight());
	previewToolbar->Refresh();
	sliderVideo->SetSize(0, pictureHeight + sliderVideo->GetHeight(), width, sliderVideo->GetHeight());
	sliderVideo->Refresh();
}


bool CShowPreview::SetBitmap(CImageLoadingFormat* bitmap)
{
	if (previewToolbar != nullptr)
		previewToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());

	if (bitmapWindow != nullptr)
	{
		bitmap->SetOrientation(orientation);
		bitmapWindow->SetBitmap(bitmap, false);
	}

	return true;
}


void CShowPreview::OnViewerZoomIn(wxCommandEvent& event)
{
	if (previewToolbar != nullptr)
		previewToolbar->ChangeZoomInPos();
}

void CShowPreview::OnViewerZoomOut(wxCommandEvent& event)
{
	if (previewToolbar != nullptr)
		previewToolbar->ChangeZoomOutPos();
}
