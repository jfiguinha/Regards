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
#include <FFmpegTranscodingPimpl.h>
#include <FFmpegDecodeFrame.h>
#include <FileUtility.h>
#include "CompressionAudioVideoOption.h"
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

CShowPreview::CShowPreview(wxWindow* parent, wxWindowID id, wxWindowID bitmapViewerId,
	wxWindowID mainViewerId, CThemeParam * config,
	const wxString &videoFilename, COpenCLEngine * openCLEngine, CVideoOptionCompress * videoOptionCompress)
	: CWindowMain("ShowBitmap", parent, id)
{
	transitionEnd = false;
	tempImage = nullptr;
	scrollbar = nullptr;
	previewToolbar = nullptr;
	bitmapWindow = nullptr;
	configRegards = nullptr;
	defaultToolbar = true;
	defaultViewer = true;
	this->videoOptionCompress = *videoOptionCompress;
	this->openCLEngine = openCLEngine;

	

	CThemeBitmapWindow themeBitmap;
	configRegards = CParamInit::getInstance();
	CThemeScrollBar themeScroll;
	CThemeToolbar themeToolbar;
	CThemeSlider themeSlider;
	std::vector<int> value = { 1, 2, 3, 4, 5, 6, 8, 12, 16, 25, 33, 50, 66, 75, 100, 133, 150, 166, 200, 300, 400, 500, 600, 700, 800, 1200, 1600 };


	if (config != nullptr)
	{
		config->GetBitmapToolbarTheme(&themeToolbar);
	}

	previewToolbar = nullptr;

	previewToolbar = new CPreviewToolbar(this, wxID_ANY, bitmapViewerId, themeToolbar, false);
	previewToolbar->SetTabValue(value);

	if (config != nullptr)
		config->GetBitmapWindowTheme(&themeBitmap);

	themeBitmap.colorScreen = wxColour("black");

	bitmapWindow = new CBitmapWnd(this, bitmapViewerId, previewToolbar, 0, themeBitmap);
	
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

	Connect(wxEVENT_SHOWORIGINAL, wxCommandEventHandler(CShowPreview::OnShowOriginal));
	Connect(wxEVENT_SHOWNEW, wxCommandEventHandler(CShowPreview::OnShowNew));

	Connect(wxEVENT_UPDATEPICTURE, wxCommandEventHandler(CShowPreview::OnUpdatePicture));

	wxString decoder = "";
	/*
	CRegardsConfigParam * regardsParam = CParamInit::getInstance();
	if (regardsParam != nullptr)
	{
		decoder = regardsParam->GetVideoDecoderHardware();
	}
	*/
	progressValue = 0;
	filename = videoFilename;

	decodeFrameOriginal = new CFFmpegDecodeFrame(decoder);
	decodeFrameOriginal->OpenFile(filename);
	timeTotal = decodeFrameOriginal->GetTotalTime();
	
	sliderVideo->SetTotalSecondTime(timeTotal * 1000);

	wxString resourcePath = CFileUtility::GetResourcesFolderPath();
	
	//m_animation->LoadFile(resourcePath + "/loading.gif");

	//this->SetSize(640, 480);
}

void CShowPreview::ShowPicture(CFFmpegDecodeFrame * decodeFrame, const wxString &label)
{
	CRegardsBitmap * bitmap = decodeFrame->GetBitmap(false);
	if (bitmap != nullptr && bitmap->GetBitmapWidth() > 0 && bitmap->GetBitmapHeight())
	{
		CImageLoadingFormat * imageLoadingFormat = new CImageLoadingFormat(false);
		imageLoadingFormat->SetPicture(bitmap);
		if (isFirstPicture)
			bitmapWindow->SetBitmap(imageLoadingFormat, false);
		else
			bitmapWindow->UpdateBitmap(imageLoadingFormat, false);

		CompressionAudioVideoOption * dlg = (CompressionAudioVideoOption *)this->FindWindowByName("CompressionAudioVideoOption");
		dlg->ChangeLabelPicture(label);

		isFirstPicture = false;
	}

}

void CShowPreview::ShowOriginal()
{
	ShowPicture(decodeFrameOriginal, "Original Video");
}

void CShowPreview::ShowNew()
{
	ShowPicture(decodeFrame, "Export Video");
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

void CShowPreview::SlidePosChange(const int &position, const wxString &key)
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

void CShowPreview::MoveSlider(const int64_t &position)
{
    showOriginal = oldShowOriginal;
	moveSlider = false;
	this->position = position;
	UpdateBitmap(nullptr,"");
}

void CShowPreview::ThreadLoading(void * data)
{
	int ret = 0;
	CShowPreview * showPreview = (CShowPreview *)data;

	CImageLoadingFormat * imageLoadingFormat = nullptr;
	CRegardsBitmap * picture = nullptr;

	wxMemoryOutputStream dataOutput;

	wxString fileTemp = "";

	if (!showPreview->moveSlider)
	{
		
		fileTemp = CFileUtility::GetTempFile("video_temp." + showPreview->extension, true);
		//fileTemp = CFileUtility::GetTempFile("video_temp.mkv", true);

		ret = showPreview->transcodeFFmpeg->EncodeOneFrame(&dataOutput, showPreview->filename, fileTemp, showPreview->position, &showPreview->videoOptionCompress);
		//showPreview->transcodeFFmpeg->EncodeOneFrame(nullptr, showPreview->filename, fileTemp, showPreview->position, &showPreview->videoOptionCompress);
		showPreview->transcodeFFmpeg->EndTreatment();

		if (ret == 0)
		{
			showPreview->compressIsOK = true;

			showPreview->decodeFrame->OpenFile(&dataOutput, fileTemp);
			//showPreview->decodeFrame->OpenFile(fileTemp);
			showPreview->decodeFrame->GetFrameBitmapPosition(0);
			CRegardsBitmap * bmp = showPreview->decodeFrame->GetBitmap(false);
			if (bmp != nullptr)
				bmp->VertFlipBuf();
			showPreview->decodeFrame->EndTreatment();

		}
		else
			showPreview->compressIsOK = false;

		showPreview->decodeFrameOriginal->GetFrameBitmapPosition(showPreview->position);
		CRegardsBitmap * bmp = showPreview->decodeFrameOriginal->GetBitmap(false);
		if (bmp != nullptr)
			bmp->VertFlipBuf();

		if (showPreview->key == "Click")
		{
			showPreview->decodeFrameOriginal->GetFrameBitmapPosition(showPreview->position);
			bmp = showPreview->decodeFrameOriginal->GetBitmap(false);
			if (bmp != nullptr)
				bmp->VertFlipBuf();
		}

		wxCommandEvent evt(wxEVENT_UPDATEPICTURE);
		showPreview->GetEventHandler()->AddPendingEvent(evt);

	}
	else
	{
		showPreview->decodeFrameOriginal->GetFrameBitmapPosition(showPreview->position);
		CRegardsBitmap * bmp = showPreview->decodeFrameOriginal->GetBitmap(false);
		if (bmp != nullptr)
			bmp->VertFlipBuf();




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

void CShowPreview::UpdateBitmap(CVideoOptionCompress * videoOptionCompress, const wxString & extension, const bool &updatePicture)
{
	wxString decoder = "";

	if (videoOptionCompress != nullptr)
	{
		this->videoOptionCompress = *videoOptionCompress;
		this->extension = extension;
	}

	if (transcodeFFmpeg == nullptr)
		transcodeFFmpeg = new CFFmpegTranscodingPimpl(openCLEngine, decoder);

	if (decodeFrame == nullptr)
		decodeFrame = new CFFmpegDecodeFrame(decoder);

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
	delete(bitmapWindow);
	delete(scrollbar);

	if(transcodeFFmpeg != nullptr)
		delete transcodeFFmpeg;
	if (decodeFrame != nullptr)
		delete decodeFrame;
	if (decodeFrameOriginal != nullptr)
		delete decodeFrameOriginal;

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


bool CShowPreview::SetBitmap(CImageLoadingFormat * bitmap)
{
	TRACE();

	if (previewToolbar != nullptr)
		previewToolbar->SetTrackBarPosition(bitmapWindow->GetPosRatio());

	if (bitmapWindow != nullptr)
		bitmapWindow->SetBitmap(bitmap, false);



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

