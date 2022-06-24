#include <header.h>
#include "CompressionAudioVideoOption.h"
#include <VideoCompressOption.h>
#include <wx/filename.h>
#include <libPicture.h>
#include "SliderVideoSelection.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <RegardsConfigParam.h>
#include <videothumb.h>
#include "ShowPreview.h"
#include <LibResource.h>
#include <RegardsBitmap.h>
#include <WindowUtility.h>
extern "C" {
#include <libavutil/error.h>
}



#ifndef WX_PRECOMP
//(*InternalHeadersPCH(CompressionAudioVideoOption)
//*)
#endif
//(*InternalHeaders(CompressionAudioVideoOption)
#include <wx/xrc/xmlres.h>
#include <wx/statbmp.h>
#include <wx/spinctrl.h>
//*)

//(*IdInit(CompressionAudioVideoOption)
//*)

BEGIN_EVENT_TABLE(CompressionAudioVideoOption, wxDialog)
	//(*EventTable(CompressionAudioVideoOption)
	//*)
END_EVENT_TABLE()

using namespace Regards::Picture;
using namespace Regards::Video;

static void GetTimeToHourMinuteSecond(const long& timeToSplit, int& hour, int& minute, int& second)
{
	long timeToSplitlocal = timeToSplit;
	hour = timeToSplit / 3600;
	timeToSplitlocal = timeToSplitlocal - (hour * 3600);
	minute = timeToSplitlocal / 60;
	timeToSplitlocal = timeToSplitlocal - (minute * 60);
	second = timeToSplitlocal;
}

CompressionAudioVideoOption::CompressionAudioVideoOption(wxWindow* parent)
{
	isOk = false;
	videoEffectParameter = new CVideoEffectParameter();
	//(*Initialize(CompressionAudioVideoOption)
	wxXmlResource::Get()->LoadObject(this, parent,_T("CompressionAudioVideoOption"),_T("wxDialog"));
	btnCancel = static_cast<wxButton*>(FindWindow(XRCID("ID_BTCANCEL")));
	btnOk = static_cast<wxButton*>(FindWindow(XRCID("ID_BTOK")));
	btnPreview = static_cast<wxButton*>(FindWindow(XRCID("ID_BTPREVIEW")));
	ckVideoHardware = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKVIDEOHARDWARE")));
	ckAudioBitRate = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKAUDIOBITRATE")));
	cbAudioBitRate = static_cast<wxComboBox*>(FindWindow(XRCID("ID_CBAUDIOBITRATE")));
	ckAudioQuality = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKAUDIOQUALITY")));
	cbAudioQuality = static_cast<wxComboBox*>(FindWindow(XRCID("ID_CBAUDIOQUALITY")));
	cbAudioCodec = static_cast<wxComboBox*>(FindWindow(XRCID("ID_CBAUDIOCODEC")));
	cbVideoCodec = static_cast<wxComboBox*>(FindWindow(XRCID("ID_CBVIDEOCODEC")));
	cbVideoPreset = static_cast<wxComboBox*>(FindWindow(XRCID("ID_CBVIDEOPRESET")));
	cbVideoProfile = static_cast<wxComboBox*>(FindWindow(XRCID("ID_CBVIDEOPROFILE")));
	rbQuality = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBQUALITY")));
	ckVideoQuality = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKVIDEOQUALITY")));
	slCompression = static_cast<wxSlider*>(FindWindow(XRCID("ID_SLCOMPRESSIONQUALITY")));
	ckVideoBitRate = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKVIDEOBITRATE")));
	txtBitRate = static_cast<wxTextCtrl*>(FindWindow(XRCID("ID_TXTBITRATE")));
	bitmap = static_cast<wxStaticBitmap*>(FindWindow(XRCID("ID_BITMAPVIDEO")));
	ckVideoAutocontrast = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKVIDEOAUTOCONTRAST")));
	ckVideoStabilization = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKVIDEOSTABILIZATION")));
#ifdef USE_PREVIEW_INTEGRATE
#ifndef __APPLE__
	bitmapPreview = static_cast<wxStaticBitmap*>(FindWindow(XRCID("ID_BITMAPVIDEOPREVIEW")));
#endif
#endif

	stPreviewPicture = static_cast<wxStaticBox*>(FindWindow(XRCID("ID_STPREVIEWPICTURE")));
	labelTimeStart = static_cast<wxTimePickerCtrl*>(FindWindow(XRCID("ID_STSTARTMOVIE")));
	labelTimeEnd = static_cast<wxTimePickerCtrl*>(FindWindow(XRCID("ID_STENDMOVIE")));
	slVideo = static_cast<wxSlider*>(FindWindow(XRCID("ID_SLVIDEO")));

	//Filter
	ckdenoiseFilter = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKDENOISEFILTER")));
	denoiseFilter = static_cast<wxSlider*>(FindWindow(XRCID("ID_SLDENOISEFILTER")));
	cksharpenFilter = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKSHARPENFILTER")));
	sharpenFilter = static_cast<wxSlider*>(FindWindow(XRCID("ID_SLSHARPENFILTER")));
	cklightandcontrast = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKCONTRASTANDLIGHTFILTER")));
	contrastFilter = static_cast<wxSlider*>(FindWindow(XRCID("ID_SLCONTRASTFILTER")));
	lightFilter = static_cast<wxSlider*>(FindWindow(XRCID("ID_SLLIGHTFILTER")));
	ckcolorBoost = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKDCOLORBOOSTFILTER")));
	redFilter = static_cast<wxSlider*>(FindWindow(XRCID("ID_SLREDBOOSTFILTER")));
	greenFilter = static_cast<wxSlider*>(FindWindow(XRCID("ID_SLGREENBOOSTFILTER")));
	blueFilter = static_cast<wxSlider*>(FindWindow(XRCID("ID_SLBLUEBOOSTFILTER")));
	ckgrey = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKGRAYFILTER")));
	cksepia = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKSEPIAFILTER")));
	cknoise = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKNOISEFILTER")));
	ckenablefilter = static_cast<wxCheckBox*>(FindWindow(XRCID("ID_CKENABLEFILTER")));
	rbAudioDirectCopy = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBAUDIOCOMPRESSION")));
	rbVideoDirectCopy = static_cast<wxRadioBox*>(FindWindow(XRCID("ID_RBVIDEOCOMPRESSION")));

	//bufferStabilization = (wxSpinCtrl*)FindWindow(XRCID("ID_SPINCONTROLBUFFER"));
#ifdef USE_PREVIEW_INTEGRATE
	panel = static_cast<wxPanel*>(FindWindow(XRCID("IDPANEL")));
#endif
	Connect(XRCID("ID_SLDENOISEFILTER"), wxEVT_SCROLL_THUMBTRACK,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnSliderFilterClick);
	Connect(XRCID("ID_SLSHARPENFILTER"), wxEVT_SCROLL_THUMBTRACK,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnSliderFilterClick);
	Connect(XRCID("ID_SLCONTRASTFILTER"), wxEVT_SCROLL_THUMBTRACK,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnSliderFilterClick);
	Connect(XRCID("ID_SLLIGHTFILTER"), wxEVT_SCROLL_THUMBTRACK,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnSliderFilterClick);
	Connect(XRCID("ID_SLREDBOOSTFILTER"), wxEVT_SCROLL_THUMBTRACK,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnSliderFilterClick);
	Connect(XRCID("ID_SLGREENBOOSTFILTER"), wxEVT_SCROLL_THUMBTRACK,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnSliderFilterClick);
	Connect(XRCID("ID_SLBLUEBOOSTFILTER"), wxEVT_SCROLL_THUMBTRACK,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnSliderFilterClick);
	Connect(XRCID("ID_SLBLUEBOOSTFILTER"), wxEVT_SCROLL_THUMBTRACK,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnSliderFilterClick);

	Connect(XRCID("ID_CKDENOISEFILTER"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckFilterClick);
	Connect(XRCID("ID_CKDENOISEFILTER"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckFilterClick);
	Connect(XRCID("ID_CKSHARPENFILTER"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckFilterClick);
	Connect(XRCID("ID_CKCONTRASTANDLIGHTFILTER"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckFilterClick);
	Connect(XRCID("ID_CKDCOLORBOOSTFILTER"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckFilterClick);
	Connect(XRCID("ID_CKGRAYFILTER"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckFilterClick);
	Connect(XRCID("ID_CKSEPIAFILTER"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckFilterClick);
	Connect(XRCID("ID_CKNOISEFILTER"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckFilterClick);
	Connect(XRCID("ID_CKENABLEFILTER"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckFilterClick);

	Connect(XRCID("ID_CKAUDIOBITRATE"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckAudioBitrateClick);
	Connect(XRCID("ID_CKAUDIOQUALITY"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckAudioQualityClick);
	Connect(XRCID("ID_CKVIDEOQUALITY"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckVideoQualityClick);
	Connect(XRCID("ID_CKVIDEOBITRATE"), wxEVT_CHECKBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckVideoBitrateClick);
	Connect(XRCID("ID_BTCANCEL"),wxEVT_COMMAND_BUTTON_CLICKED,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCancelClick);
	Connect(XRCID("ID_BTOK"), wxEVT_COMMAND_BUTTON_CLICKED,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnOkClick);
	Connect(XRCID("ID_BTPREVIEW"), wxEVT_COMMAND_BUTTON_CLICKED,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnPreviewClick);
	//Connect(XRCID("ID_SLVIDEO"), wxEVT_SCROLL_CHANGED, (wxObjectEventFunction)&CompressionAudioVideoOption::OnVideoSliderChange);
	Connect(wxEVENT_SETVIDEODURATION, wxCommandEventHandler(CompressionAudioVideoOption::OnSetVideoDuration));
	Connect(wxEvent_SLIDERMOVE, wxCommandEventHandler(CompressionAudioVideoOption::OnVideoSliderChange));
	Connect(wxEVENT_CLOSEPREVIEW, wxCommandEventHandler(CompressionAudioVideoOption::OnClosePreview));
	Connect(wxEVENT_ERRORCOMPRESSION, wxCommandEventHandler(CompressionAudioVideoOption::OnErrorCompression));
	Connect(XRCID("ID_CBVIDEOCODEC"), wxEVT_COMBOBOX,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnVideoCodecSelect);

	wxString decoder = "";
	



	CThemeSlider theme;
	CMainTheme* viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		viewerTheme->GetVideoSliderTheme(&theme);
		theme.font.SetColorFont(*wxBLACK);
	}
	wxColour bgColor = labelTimeStart->GetParent()->GetBackgroundColour();
	sliderVideoPosition = new CSliderVideoSelection(labelTimeStart->GetParent(), wxID_ANY, this, theme);
	auto size = wxSize(slVideo->GetSize().x, theme.GetHeight());
	sliderVideoPosition->SetSize(size);
	sliderVideoPosition->SetPosition(slVideo->GetPosition());
	sliderVideoPosition->SetBackgroundColour(bgColor);
	slVideo->Show(false);

	Connect(XRCID("ID_STSTARTMOVIE"), wxEVT_TIME_CHANGED,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnSlideFromChange);
	Connect(XRCID("ID_STENDMOVIE"), wxEVT_TIME_CHANGED,
	        (wxObjectEventFunction)&CompressionAudioVideoOption::OnSlideToChange);


	CThemeBitmapWindow themeBitmap;
	showBitmapWindow = nullptr;
	//CMainTheme * viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
		viewerTheme->GetBitmapWindowTheme(&themeBitmap);



#ifdef __APPLE__
	showBitmapWindow = new CShowPreview(this, SHOWBITMAPVIEWERDLGID, viewerTheme);
	showBitmapWindow->Show(true);
	showBitmapWindow->SetSize(panel->GetPosition().x + 20, panel->GetPosition().y + 25, panel->GetSize().x - 40, panel->GetSize().y - 100);
#else
	showBitmapWindow = new CShowPreview(panel, SHOWBITMAPVIEWERDLGID, viewerTheme);
	showBitmapWindow->Show(true);
	showBitmapWindow->SetSize(bitmapPreview->GetPosition().x, bitmapPreview->GetPosition().y,
	                          bitmapPreview->GetSize().x, bitmapPreview->GetSize().y);
	bitmapPreview->Show(false);
	panel->Show(true);
	
#endif

}

void CompressionAudioVideoOption::SetBitmap(const long& pos)
{
	int orientation = 0;// ffmpegTranscoding->GetVideoOrientation();
	CRegardsBitmap* bitmap_local = ffmpegTranscoding->GetVideoFramePos(pos, 340, 240);
	bitmap_local->RotateExif(orientation);
	wxImage picture = CLibPicture::ConvertRegardsBitmapToWXImage(bitmap_local);
	int x = 0;
	int y = 0;
	x = (340 - picture.GetWidth()) / 2;
	y = (240 - picture.GetHeight()) / 2;

	wxBitmap test_bitmap(340, 240);
	wxMemoryDC temp_dc;
	temp_dc.SelectObject(test_bitmap);
	CWindowUtility winUtility;
	winUtility.FillRect(&temp_dc, wxRect(0,0,340,240), *wxBLACK);
	temp_dc.DrawBitmap(picture, x, y);
	temp_dc.SelectObject(wxNullBitmap);
	bitmap->SetBitmap(test_bitmap);

	delete bitmap_local;
}


void CompressionAudioVideoOption::SetFile(const wxString& videoFilename,
	const wxString& videoOutputFilename)
{
	CVideoOptionCompress videoOptionCompress;
	GetCompressionOption(&videoOptionCompress);
	showBitmapWindow->SetParameter(videoFilename, &videoOptionCompress);
	//showBitmapWindow->UpdateBitmap(&videoOptionCompress, extension);

	
	wxFileName filepath(videoOutputFilename);
	extension = filepath.GetExt();
	if (extension == "mpeg")
	{
		cbVideoCodec->Clear();
		cbVideoCodec->AppendString("MPEG2");
		cbVideoCodec->SetStringSelection("MPEG2");
		cbVideoProfile->SetStringSelection("None");
	}
	else if (extension == "mov")
	{
		cbVideoCodec->Clear();
		cbVideoCodec->AppendString("H264");
		cbVideoCodec->AppendString("H265");
		cbVideoCodec->SetStringSelection("H264");
		cbVideoProfile->SetStringSelection("Main");

		cbAudioCodec->Clear();
		cbAudioCodec->AppendString("AAC");
		cbAudioCodec->SetStringSelection("AAC");
	}
	else if (extension == "mp4")
	{
		cbVideoCodec->Clear();
		cbVideoCodec->AppendString("H264");
		cbVideoCodec->AppendString("H265");
		cbVideoCodec->AppendString("MPEG4");
		cbVideoCodec->SetStringSelection("H264");
		cbVideoProfile->SetStringSelection("Main");

		cbAudioCodec->Clear();
		cbAudioCodec->AppendString("AAC");
		cbAudioCodec->AppendString("MP3");
		cbAudioCodec->SetStringSelection("AAC");
	}
	else if (extension == "webm")
	{
		cbVideoCodec->Clear();
		cbVideoCodec->AppendString("VP8");
		cbVideoCodec->AppendString("VP9");
		cbVideoCodec->SetStringSelection("VP9");
		cbVideoProfile->SetStringSelection("Auto");

		cbAudioCodec->Clear();
		cbAudioCodec->AppendString("VORBIS");
		cbAudioCodec->SetStringSelection("VORBIS");
	}

	if (ffmpegTranscoding != nullptr)
		delete ffmpegTranscoding;

	ffmpegTranscoding = new Regards::Video::CThumbnailVideo(videoFilename);
	timeTotal = ffmpegTranscoding->GetMovieDuration();
	slVideo->SetMax(timeTotal);

	//labelTimeEnd->SetRange(0, timeTotal);
	labelTimeStart->SetTime(0, 0, 0);

	int hour = 0;
	int minute = 0;
	int second = 0;
	GetTimeToHourMinuteSecond(timeTotal, hour, minute, second);
	labelTimeEnd->SetTime(hour, minute, second);

	sliderVideoPosition->SetTotalSecondTime(timeTotal);



	SetBitmap(0);

}

void CompressionAudioVideoOption::OnErrorCompression(wxCommandEvent& event)
{
	int errorCode = event.GetInt();
	wxString errorConversion = CLibResource::LoadStringFromResource("LBLERRORCONVERSION", 1);
	char message[255];
	av_make_error_string(message, AV_ERROR_MAX_STRING_SIZE, errorCode);
	wxMessageBox(message, errorConversion, wxICON_ERROR);
	this->Close();
}

void CompressionAudioVideoOption::OnClosePreview(wxCommandEvent& event)
{
#ifndef USE_PREVIEW_INTEGRATE
	btnPreview->SetLabelText("Preview");
	previewDlg->Hide();
#endif
}

void CompressionAudioVideoOption::OnVideoCodecSelect(wxCommandEvent& event)
{
	wxString codec = cbVideoCodec->GetStringSelection();
	cbVideoProfile->Clear();
	cbVideoPreset->Clear();
	if (codec == "H264")
	{
		cbVideoProfile->AppendString("Auto");
		cbVideoProfile->AppendString("High");
		cbVideoProfile->AppendString("Main");
		cbVideoProfile->AppendString("Baseline");

		cbVideoPreset->AppendString("UltraFast");
		cbVideoPreset->AppendString("SuperFast");
		cbVideoPreset->AppendString("VeryFast");
		cbVideoPreset->AppendString("Faster");
		cbVideoPreset->AppendString("Fast");
		cbVideoPreset->AppendString("Medium");
		cbVideoPreset->AppendString("Slow");
		cbVideoPreset->AppendString("Slower");
		cbVideoPreset->AppendString("VerySlow");
		cbVideoPreset->AppendString("Placebo");

		cbVideoPreset->SetLabel("Medium");
		cbVideoProfile->SetLabel("Main");
	}
	else if (codec == "H265")
	{
		cbVideoProfile->AppendString("Auto");
		cbVideoProfile->AppendString("Main");
		cbVideoProfile->AppendString("Main Still Picture");
		cbVideoPreset->AppendString("None");
		/*
		cbVideoPreset->AppendString("UltraFast");
		cbVideoPreset->AppendString("SuperFast");
		cbVideoPreset->AppendString("VeryFast");
		cbVideoPreset->AppendString("Faster");
		cbVideoPreset->AppendString("Fast");
		cbVideoPreset->AppendString("Medium");
		cbVideoPreset->AppendString("Slow");
		cbVideoPreset->AppendString("Slower");
		cbVideoPreset->AppendString("VerySlow");
		cbVideoPreset->AppendString("Placebo");
		*/
		cbVideoPreset->SetLabel("None");
		cbVideoProfile->SetLabel("Main");
	}
	else if (codec == "MPEG4")
	{
		cbVideoProfile->AppendString("Auto");
		cbVideoPreset->AppendString("None");
		cbVideoPreset->SetLabel("None");
		cbVideoProfile->SetLabel("Auto");
	}
	else if (codec == "MPEG2")
	{
		cbVideoProfile->AppendString("Auto");
		cbVideoPreset->AppendString("None");
		cbVideoPreset->SetLabel("None");
		cbVideoProfile->SetLabel("Auto");
	}
	else if (codec == "VP8" || codec == "VP9")
	{
		cbVideoProfile->AppendString("Auto");
		cbVideoPreset->AppendString("VeryFast");
		cbVideoPreset->AppendString("Faster");
		cbVideoPreset->AppendString("Fast");
		cbVideoPreset->AppendString("Medium");
		cbVideoPreset->AppendString("Slow");
		cbVideoPreset->AppendString("Slower");
		cbVideoPreset->AppendString("VerySlow");

		cbVideoPreset->SetLabel("Medium");
		cbVideoProfile->SetLabel("Auto");
	}
}

void CompressionAudioVideoOption::OnbtnSliderFilterClick(wxScrollEvent& event)
{
	//CVideoEffectParameter videoEffectParameter;
	videoEffectParameter->effectEnable = ckenablefilter->GetValue();
	videoEffectParameter->denoiseEnable = ckdenoiseFilter->GetValue();
	videoEffectParameter->denoisingLevel = denoiseFilter->GetValue();
	videoEffectParameter->SharpenEnable = cksharpenFilter->GetValue();
	videoEffectParameter->grayEnable = ckgrey->GetValue();
	videoEffectParameter->sepiaEnable = cksepia->GetValue();
	videoEffectParameter->filmgrainenable = cknoise->GetValue();
	videoEffectParameter->contrast = contrastFilter->GetValue();
	videoEffectParameter->brightness = lightFilter->GetValue();
	videoEffectParameter->ColorBoostEnable = ckcolorBoost->GetValue();
	videoEffectParameter->color_boost[0] = redFilter->GetValue();
	videoEffectParameter->color_boost[1] = greenFilter->GetValue();
	videoEffectParameter->color_boost[2] = blueFilter->GetValue();
	videoEffectParameter->bandcEnable = cklightandcontrast->GetValue();
	videoEffectParameter->sharpness = sharpenFilter->GetValue() / 10.0f;
}

void CompressionAudioVideoOption::OnbtnCheckFilterClick(wxCommandEvent& event)
{
	//CVideoEffectParameter videoEffectParameter;
	videoEffectParameter->effectEnable = ckenablefilter->GetValue();
	videoEffectParameter->denoiseEnable = ckdenoiseFilter->GetValue();
	videoEffectParameter->denoisingLevel = denoiseFilter->GetValue();
	videoEffectParameter->SharpenEnable = cksharpenFilter->GetValue();
	videoEffectParameter->grayEnable = ckgrey->GetValue();
	videoEffectParameter->sepiaEnable = cksepia->GetValue();
	videoEffectParameter->filmgrainenable = cknoise->GetValue();
	videoEffectParameter->contrast = contrastFilter->GetValue();
	videoEffectParameter->brightness = lightFilter->GetValue();
	videoEffectParameter->ColorBoostEnable = ckcolorBoost->GetValue();
	videoEffectParameter->color_boost[0] = redFilter->GetValue();
	videoEffectParameter->color_boost[1] = greenFilter->GetValue();
	videoEffectParameter->color_boost[2] = blueFilter->GetValue();
	videoEffectParameter->bandcEnable = cklightandcontrast->GetValue();
	videoEffectParameter->sharpness = sharpenFilter->GetValue() / 10.0f;
}

void CompressionAudioVideoOption::OnbtnPreviewClick(wxCommandEvent& event)
{
#ifdef USE_PREVIEW_INTEGRATE
	CVideoOptionCompress videoOptionCompress;
	GetCompressionOption(&videoOptionCompress);
	showBitmapWindow->UpdateBitmap(&videoOptionCompress, extension);
#else

	if (previewDlg->IsShown())
	{
		CVideoOptionCompress videoOptionCompress;
		GetCompressionOption(&videoOptionCompress);
		previewDlg->UpdatePreview(&videoOptionCompress, extension);
	}
	else
	{

		CVideoOptionCompress videoOptionCompress;
		GetCompressionOption(&videoOptionCompress);
		previewDlg->UpdatePreview(&videoOptionCompress, extension);
		previewDlg->Show();
		wxPoint pt = this->GetPosition();
		wxSize size = this->GetSize();
		pt.x += size.GetWidth();
		previewDlg->SetPosition(pt);
		btnPreview->SetLabelText("Refresh");
	}
#endif
}

void CompressionAudioVideoOption::ChangeLabelPicture(const wxString& label)
{
	stPreviewPicture->SetLabel(label);
}

void CompressionAudioVideoOption::OnSlideFromChange(wxDateEvent& event)
{
	wxDateTime pos = event.GetDate();
	const int hour = pos.GetHour();
	const int minute = pos.GetMinute();
	const int second = pos.GetSecond();
	long _timeTotal = hour * 3600 + minute * 60 + second;
	if (_timeTotal >= 0 && _timeTotal < sliderVideoPosition->GetTimeEnd())
	{
		sliderVideoPosition->SetStartTime(_timeTotal);
	}
	else
	{
		_timeTotal = sliderVideoPosition->GetTimeEnd() - 1;
		int hour1 = 0;
		int min = 0;
		int sec = 0;
		GetTimeToHourMinuteSecond(_timeTotal, hour1, min, sec);
		labelTimeStart->SetTime(hour1, min, sec);
	}
	SetBitmap(_timeTotal);
}

void CompressionAudioVideoOption::OnSlideToChange(wxDateEvent& event)
{
	wxDateTime pos = event.GetDate();
	int hour = pos.GetHour();
	int minute = pos.GetMinute();
	int second = pos.GetSecond();
	long _timeTotal = hour * 3600 + minute * 60 + second;
	if (_timeTotal <= timeTotal && _timeTotal > sliderVideoPosition->GetTimeStart())
	{
		sliderVideoPosition->SetEndTime(_timeTotal);
	}
	else
	{
		_timeTotal = sliderVideoPosition->GetTimeStart() + 1;
		int hour1 = 0;
		int min = 0;
		int sec = 0;
		GetTimeToHourMinuteSecond(_timeTotal, hour1, min, sec);
		labelTimeEnd->SetTime(hour1, min, sec);
	}
	SetBitmap(_timeTotal);
}

void CompressionAudioVideoOption::OnVideoSliderChange(wxCommandEvent& event)
{
	int type = event.GetInt();
	long value = event.GetExtraLong();

	SetBitmap(value);

	if (type == 1)
	{
		int hour = 0;
		int minute = 0;
		int second = 0;
		GetTimeToHourMinuteSecond(value, hour, minute, second);
		labelTimeStart->SetTime(hour, minute, second);
	}

	if (type == 2)
	{
		int hour = 0;
		int minute = 0;
		int second = 0;
		GetTimeToHourMinuteSecond(value, hour, minute, second);
		labelTimeEnd->SetTime(hour, minute, second);
	}
}

CompressionAudioVideoOption::~CompressionAudioVideoOption()
{
	if (ffmpegTranscoding)
	{
		delete ffmpegTranscoding;
		ffmpegTranscoding = nullptr;
	}

	if (sliderVideoPosition)
	{
		delete sliderVideoPosition;
		sliderVideoPosition = nullptr;
	}

	if (showBitmapWindow != nullptr)
		delete showBitmapWindow;

	if (videoEffectParameter != nullptr)
		delete videoEffectParameter;
}

wxString CompressionAudioVideoOption::ConvertSecondToTime(int64_t sec)
{
	int h = (sec / 3600);
	int m = (sec - (3600 * h)) / 60;
	int s = (sec - (3600 * h) - (m * 60));
	return wxString::Format("%02d:%02d:%02d\n", h, m, s);
}

void CompressionAudioVideoOption::OnSetVideoDuration(wxCommandEvent& event)
{
	int64_t duration = event.GetExtraLong();
	int hour = 0;
	int minute = 0;
	int second = 0;
	GetTimeToHourMinuteSecond(duration, hour, minute, second);
	labelTimeEnd->SetTime(hour, minute, second);
}

void CompressionAudioVideoOption::OnbtnCheckAudioBitrateClick(wxCommandEvent& event)
{
	if (ckAudioBitRate->IsChecked())
		ckAudioQuality->SetValue(false);
}

void CompressionAudioVideoOption::OnbtnCheckAudioQualityClick(wxCommandEvent& event)
{
	if (ckAudioQuality->IsChecked())
		ckAudioBitRate->SetValue(false);
}

void CompressionAudioVideoOption::OnbtnCheckVideoQualityClick(wxCommandEvent& event)
{
	if (ckVideoQuality->IsChecked())
		ckVideoBitRate->SetValue(false);
}

void CompressionAudioVideoOption::OnbtnCheckVideoBitrateClick(wxCommandEvent& event)
{
	if (ckVideoBitRate->IsChecked())
		ckVideoQuality->SetValue(false);
}

bool CompressionAudioVideoOption::IsOk()
{
	return isOk;
}

void CompressionAudioVideoOption::GetCompressionOption(CVideoOptionCompress* videoOptionCompress)
{
	if (videoOptionCompress != nullptr)
	{
		//bufferStabilization
		//videoOptionCompress->videoEffectParameter.stabilizeImageBuffere = bufferStabilization->GetValue();
		videoOptionCompress->videoEffectParameter.autoConstrast = ckVideoAutocontrast->GetValue();
		videoOptionCompress->videoEffectParameter.stabilizeVideo = ckVideoStabilization->GetValue();
		videoOptionCompress->videoEffectParameter.effectEnable = ckenablefilter->GetValue();
		videoOptionCompress->videoEffectParameter.denoiseEnable = ckdenoiseFilter->GetValue();
		videoOptionCompress->videoEffectParameter.denoisingLevel = denoiseFilter->GetValue();
		videoOptionCompress->videoEffectParameter.SharpenEnable = cksharpenFilter->GetValue();
		videoOptionCompress->videoEffectParameter.grayEnable = ckgrey->GetValue();
		videoOptionCompress->videoEffectParameter.sepiaEnable = cksepia->GetValue();
		videoOptionCompress->videoEffectParameter.filmgrainenable = cknoise->GetValue();
		videoOptionCompress->videoEffectParameter.contrast = contrastFilter->GetValue();
		videoOptionCompress->videoEffectParameter.brightness = lightFilter->GetValue();
		videoOptionCompress->videoEffectParameter.ColorBoostEnable = ckcolorBoost->GetValue();

		videoOptionCompress->videoEffectParameter.color_boost[0] = redFilter->GetValue();
		videoOptionCompress->videoEffectParameter.color_boost[1] = greenFilter->GetValue();
		videoOptionCompress->videoEffectParameter.color_boost[2] = blueFilter->GetValue();
		videoOptionCompress->videoEffectParameter.bandcEnable = cklightandcontrast->GetValue();

		videoOptionCompress->videoEffectParameter.sharpness = sharpenFilter->GetValue() / 10.0f;
		videoOptionCompress->videoTime = timeTotal;
		//Audio
		videoOptionCompress->audioQualityOrBitRate = ckAudioQuality->IsChecked();
		if (cbAudioQuality->GetStringSelection() != "")
			videoOptionCompress->audioQuality = atoi(cbAudioQuality->GetStringSelection());
		else
			videoOptionCompress->audioQuality = 5;

		if (cbAudioBitRate->GetStringSelection() != "")
			videoOptionCompress->audioBitRate = atoi(cbAudioBitRate->GetStringSelection());
		else
			videoOptionCompress->audioBitRate = 128;
		videoOptionCompress->audioCodec = cbAudioCodec->GetStringSelection();

		if (videoOptionCompress->audioCodec == "")
			videoOptionCompress->audioCodec = "AAC";
		if (videoOptionCompress->audioQualityOrBitRate && videoOptionCompress->audioQuality == 0)
			videoOptionCompress->audioQuality = 5;
		else if (videoOptionCompress->audioBitRate == 0)
			videoOptionCompress->audioBitRate = 128;
		//Video
		videoOptionCompress->videoCodec = cbVideoCodec->GetStringSelection();


		if (cbVideoProfile != nullptr)
			videoOptionCompress->encoder_profile = cbVideoProfile->GetStringSelection();

		if (videoOptionCompress->encoder_profile == "")
			videoOptionCompress->encoder_profile = "main";

		if (videoOptionCompress->videoCodec == "")
			videoOptionCompress->videoCodec = "H264";

		videoOptionCompress->videoPreset = cbVideoPreset->GetStringSelection();
		if (videoOptionCompress->videoPreset == "")
			videoOptionCompress->videoPreset = "Medium";
		videoOptionCompress->constantOrVbrOption = rbQuality->GetSelection();
		videoOptionCompress->videoQualityOrBitRate = ckVideoQuality->IsChecked();
		videoOptionCompress->videoCompressionValue = slCompression->GetValue();
		videoOptionCompress->videoBitRate = atoi(txtBitRate->GetValue());
		videoOptionCompress->videoHardware = ckVideoHardware->IsChecked();
		videoOptionCompress->startTime = sliderVideoPosition->GetTimeStart();
		videoOptionCompress->audioDirectCopy = rbAudioDirectCopy->GetSelection();
		videoOptionCompress->videoDirectCopy = rbVideoDirectCopy->GetSelection();

		if (videoOptionCompress->videoDirectCopy)
			videoOptionCompress->videoEffectParameter.effectEnable = false;

		int endTime = timeTotal;
		videoOptionCompress->endTime = (sliderVideoPosition->GetTimeEnd() != endTime || sliderVideoPosition->
			                               GetTimeStart() != 0)
			                               ? sliderVideoPosition->GetTimeEnd()
			                               : 0;
	}
}

void CompressionAudioVideoOption::OnbtnOkClick(wxCommandEvent& event)
{
	if (ckVideoBitRate->IsChecked() && txtBitRate->GetValue() == "")
	{
		wxString filecompleted = CLibResource::LoadStringFromResource("LBLBITRATEVALUEERROR", 1);
		wxString infos = CLibResource::LoadStringFromResource("informationserror", 1);
		wxMessageBox(filecompleted, infos, wxICON_ERROR);
	}
	else
	{
		isOk = true;
		this->Close();
	}
}

void CompressionAudioVideoOption::OnbtnCancelClick(wxCommandEvent& event)
{
	isOk = false;
	this->Close();
}
