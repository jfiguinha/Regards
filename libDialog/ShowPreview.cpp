#include <header.h>
#include "ShowPreview.h"
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
#include <PictureData.h>
#include <MetadataExiv2.h>
#ifdef NOTENCODE_FRAME
#include <FFmpegDecodeFrameFilter.h>
#else
#include <FFmpegTranscodingPimpl.h>
#include <FFmpegDecodeFrame.h>
#include <FileUtility.h>
#endif
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

#ifdef NOTENCODE_FRAME
CShowPreview::CShowPreview(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId,
	wxWindowID mainViewerId, CThemeParam * config,
	const wxString &videoFilename, COpenCLEngine * openCLEngine, CVideoEffectParameter * videoEffectParameter)
	: CWindowMain("ShowBitmap", parent, id)
#else
CShowPreview::CShowPreview(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId,
	wxWindowID mainViewerId, CThemeParam * config,
	const wxString &videoFilename, COpenCLEngine * openCLEngine, CVideoOptionCompress * videoOptionCompress)
	: CWindowMain("ShowBitmap", parent, id)
#endif
{
	transitionEnd = false;
	tempImage = nullptr;
	scrollbar = nullptr;
	previewToolbar = nullptr;
	bitmapWindow = nullptr;
	configRegards = nullptr;
	defaultToolbar = true;
	defaultViewer = true;
#ifdef NOTENCODE_FRAME
	this->videoEffectParameter = videoEffectParameter;
#else
	this->videoOptionCompress = *videoOptionCompress;
	this->openCLEngine = openCLEngine;
#endif
	CThemeBitmapWindow themeBitmap;
	configRegards = CParamInit::getInstance();
	CThemeScrollBar themeScroll;
	CThemeToolbar themeToolbar;
	CThemeSlider themeSlider;
	std::vector<int> value = { 1, 2, 3, 4, 5, 6, 8, 12, 16, 25, 33, 50, 66, 75, 100, 133, 150, 166, 200, 300, 400, 500, 600, 700, 800, 1200, 1600 };

	this->bitmapInterface = bitmapInterface;

	if (config != nullptr)
	{
		config->GetBitmapToolbarTheme(&themeToolbar);
	}

	previewToolbar = nullptr;

	previewToolbar = new CPreviewToolbar(this, wxID_ANY, bitmapViewerId, themeToolbar, false);
	previewToolbar->SetTabValue(value);

	if (config != nullptr)
		config->GetBitmapWindowTheme(&themeBitmap);

	bitmapWindow = new CBitmapWndViewer(this, bitmapViewerId, previewToolbar, mainViewerId, themeBitmap, bitmapInterface);
	if (bitmapWindow != nullptr)
		bitmapWindow->FixArrowNavigation(false);

	if (config != nullptr)
		config->GetScrollTheme(&themeScroll);

	scrollbar = new CScrollbarWnd(this, bitmapWindow, wxID_ANY, "BitmapScroll");

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

	wxString decoder = "";
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		decoder = regardsParam->GetVideoDecoderHardware();
	}

	progressValue = 0;
	filename = videoFilename;

#ifdef NOTENCODE_FRAME
	ffmpegTranscoding = new CFFmpegDecodeFrameFilter(openCLEngine, filename, decoder);
	timeTotal = ffmpegTranscoding->GetTotalTime();
#else
	
	CFFmpegDecodeFrame * decodeFrame = new CFFmpegDecodeFrame(filename, decoder);
	timeTotal = decodeFrame->GetTotalTime();
	delete decodeFrame;
#endif
	

	
	sliderVideo->SetTotalSecondTime(timeTotal * 1000);
}

void CShowPreview::MoveSlider(const int64_t &position)
{
	this->position = position;
	UpdateBitmap(nullptr);
}

#ifdef NOTENCODE_FRAME
void CShowPreview::UpdateBitmap()
#else
void CShowPreview::UpdateBitmap(CVideoOptionCompress * videoOptionCompress)
#endif
{
	CRegardsBitmap * picture = nullptr;
#ifdef NOTENCODE_FRAME
	ffmpegTranscoding->GetFrameBitmapPosition(videoEffectParameter, position);
	picture = ffmpegTranscoding->GetBitmap();
#else

	wxString decoder = "";
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		decoder = regardsParam->GetVideoDecoderHardware();
	}

	CFFmpegTranscodingPimpl * transcodeFFmpeg = new CFFmpegTranscodingPimpl(openCLEngine, decoder);
	
	if (videoOptionCompress != nullptr)
	{
		this->videoOptionCompress = *videoOptionCompress;
	}

	wxString fileTemp = CFileUtility::GetTempFile("video_temp.mp4", true);

	transcodeFFmpeg->EncodeOneFrame(filename, fileTemp, position, &this->videoOptionCompress);

	delete transcodeFFmpeg;

	CFFmpegDecodeFrame * decodeFrame = new CFFmpegDecodeFrame(fileTemp, decoder);
	decodeFrame->GetFrameBitmapPosition(0);
	picture = decodeFrame->GetBitmap();
	delete decodeFrame;
#endif
	
	CImageLoadingFormat * imageLoadingFormat = new CImageLoadingFormat();
	imageLoadingFormat->SetPicture(picture);
	imageLoadingFormat->Flip();
	bitmapWindow->SetBitmap(imageLoadingFormat, false);
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
	delete(bitmapWindow);
	delete(scrollbar);

#ifdef NOTENCODE_FRAME
	delete ffmpegTranscoding;
#endif
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


bool CShowPreview::SetBitmap(CImageLoadingFormat * bitmap)
{
	TRACE();

	if (previewToolbar != nullptr)
		previewToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());

	if (bitmapWindow != nullptr)
		bitmapWindow->SetBitmap(bitmap, false);

	return true;
}

/*
CRegardsBitmap * CShowPreview::GetBitmap(const bool &source)
{
	if (bitmapWindow != nullptr)
		return bitmapWindow->GetBitmap(source);

	return nullptr;
}
*/
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

