#include <header.h>
#include "CompressionAudioVideoOption.h"
#include <window_id.h>
#include <VideoCompressOption.h>
#include <ImageLoadingFormat.h>
#include <videothumb.h>
#include <RegardsBitmap.h>
#include <wx/filename.h>
#include <libPicture.h>
#include "SliderVideoSelection.h"
#include "MainTheme.h"
#include "MainThemeInit.h"
#include <ParamInit.h>
#include <RegardsConfigParam.h>

#ifndef WX_PRECOMP
	//(*InternalHeadersPCH(CompressionAudioVideoOption)
	//*)
#endif
//(*InternalHeaders(CompressionAudioVideoOption)
#include <wx/xrc/xmlres.h>
#include <wx/statbmp.h>
//*)

//(*IdInit(CompressionAudioVideoOption)
//*)

BEGIN_EVENT_TABLE(CompressionAudioVideoOption, wxDialog)
//(*EventTable(CompressionAudioVideoOption)
//*)
END_EVENT_TABLE()

using namespace Regards::Picture;

static void GetTimeToHourMinuteSecond(const long &timeToSplit, int &hour, int &minute, int &second)
{
	long timeToSplitlocal = timeToSplit;
	hour = timeToSplit / 3600;
	timeToSplitlocal = timeToSplitlocal - (hour * 3600);
	minute = timeToSplitlocal / 60;
	timeToSplitlocal = timeToSplitlocal - (minute * 60);
	second = timeToSplitlocal;
}

CompressionAudioVideoOption::CompressionAudioVideoOption(wxWindow* parent, const wxString &videoFilename)
{
    isOk = false;
	this->videoFilename = videoFilename;

	//(*Initialize(CompressionAudioVideoOption)
	wxXmlResource::Get()->LoadObject(this,parent,_T("CompressionAudioVideoOption"),_T("wxDialog"));
	btnCancel = (wxButton*)FindWindow(XRCID("ID_BTCANCEL"));
	btnOk = (wxButton*)FindWindow(XRCID("ID_BTOK"));
	ckVideoHardware = (wxCheckBox*)FindWindow(XRCID("ID_CKVIDEOHARDWARE"));
	ckAudioBitRate = (wxCheckBox*)FindWindow(XRCID("ID_CKAUDIOBITRATE"));
	cbAudioBitRate = (wxComboBox*)FindWindow(XRCID("ID_CBAUDIOBITRATE"));
	ckAudioQuality = (wxCheckBox*)FindWindow(XRCID("ID_CKAUDIOQUALITY"));
	cbAudioQuality = (wxComboBox*)FindWindow(XRCID("ID_CBAUDIOQUALITY"));
	cbAudioCodec = (wxComboBox*)FindWindow(XRCID("ID_CBAUDIOCODEC"));
	cbVideoCodec = (wxComboBox*)FindWindow(XRCID("ID_CBVIDEOCODEC"));
	//cbVideoPreset = (wxComboBox*)FindWindow(XRCID("ID_CBVIDEOPRESET"));
	cbVideoProfile = (wxComboBox*)FindWindow(XRCID("ID_CBVIDEOPROFILE"));
	rbQuality = (wxRadioBox*)FindWindow(XRCID("ID_RBQUALITY"));
	ckVideoQuality = (wxCheckBox*)FindWindow(XRCID("ID_CKVIDEOQUALITY"));
	slCompression = (wxSlider*)FindWindow(XRCID("ID_SLCOMPRESSIONQUALITY"));
	ckVideoBitRate = (wxCheckBox*)FindWindow(XRCID("ID_CKVIDEOBITRATE"));
	txtBitRate = (wxTextCtrl*)FindWindow(XRCID("ID_TXTBITRATE"));
	bitmap = (wxStaticBitmap *)FindWindow(XRCID("ID_BITMAPVIDEO"));
	labelTimeStart = (wxTimePickerCtrl  *)FindWindow(XRCID("ID_STSTARTMOVIE"));
	labelTimeEnd = (wxTimePickerCtrl  *)FindWindow(XRCID("ID_STENDMOVIE"));
	slVideo = (wxSlider*)FindWindow(XRCID("ID_SLVIDEO"));
	rbAudioDirectCopy = (wxRadioBox*)FindWindow(XRCID("ID_RBAUDIOCOMPRESSION"));
	rbVideoDirectCopy = (wxRadioBox*)FindWindow(XRCID("ID_RBVIDEOCOMPRESSION"));
	//Filter
	ckdenoiseFilter = (wxCheckBox*)FindWindow(XRCID("ID_CKDENOISEFILTER"));
	denoiseFilter = (wxSlider*)FindWindow(XRCID("ID_SLDENOISEFILTER"));
	cksharpenFilter = (wxCheckBox*)FindWindow(XRCID("ID_CKSHARPENFILTER"));
	sharpenFilter = (wxSlider*)FindWindow(XRCID("ID_SLSHARPENFILTER"));
	cklightandcontrast = (wxCheckBox*)FindWindow(XRCID("ID_CKCONTRASTANDLIGHTFILTER"));
	contrastFilter = (wxSlider*)FindWindow(XRCID("ID_SLCONTRASTFILTER"));
	lightFilter = (wxSlider*)FindWindow(XRCID("ID_SLLIGHTFILTER"));
	ckcolorBoost = (wxCheckBox*)FindWindow(XRCID("ID_CKDCOLORBOOSTFILTER"));
	redFilter = (wxSlider*)FindWindow(XRCID("ID_SLREDBOOSTFILTER"));
	greenFilter = (wxSlider*)FindWindow(XRCID("ID_SLGREENBOOSTFILTER"));
	blueFilter = (wxSlider*)FindWindow(XRCID("ID_SLBLUEBOOSTFILTER"));
	ckgrey = (wxCheckBox*)FindWindow(XRCID("ID_CKGRAYFILTER"));
	cksepia = (wxCheckBox*)FindWindow(XRCID("ID_CKSEPIAFILTER"));
	cknoise = (wxCheckBox*)FindWindow(XRCID("ID_CKNOISEFILTER"));
	ckenablefilter = (wxCheckBox*)FindWindow(XRCID("ID_CKENABLEFILTER"));

	Connect(XRCID("ID_CKAUDIOBITRATE"), wxEVT_CHECKBOX, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckAudioBitrateClick);
	Connect(XRCID("ID_CKAUDIOQUALITY"), wxEVT_CHECKBOX, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckAudioQualityClick);
	Connect(XRCID("ID_CKVIDEOQUALITY"), wxEVT_CHECKBOX, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckVideoQualityClick);
	Connect(XRCID("ID_CKVIDEOBITRATE"), wxEVT_CHECKBOX, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckVideoBitrateClick);
	Connect(XRCID("ID_BTCANCEL"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCancelClick);
	Connect(XRCID("ID_BTOK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnOkClick);
	//Connect(XRCID("ID_SLVIDEO"), wxEVT_SCROLL_CHANGED, (wxObjectEventFunction)&CompressionAudioVideoOption::OnVideoSliderChange);
	Connect(wxEVENT_SETVIDEODURATION, wxCommandEventHandler(CompressionAudioVideoOption::OnSetVideoDuration));
	Connect(wxEvent_SLIDERMOVE, wxCommandEventHandler(CompressionAudioVideoOption::OnVideoSliderChange));

	ffmpegTranscoding = new CThumbnailVideo(videoFilename);
	SetBitmap(0);

	timeTotal = ffmpegTranscoding->GetMovieDuration();
	slVideo->SetMax(timeTotal);
	

	CThemeSlider theme;
	CMainTheme * viewerTheme = CMainThemeInit::getInstance();

	if (viewerTheme != nullptr)
	{
		viewerTheme->GetVideoSliderTheme(&theme);
		theme.font.SetColorFont(*wxBLACK);
	}


	//labelTimeStart->SetRange(0, timeTotal);
	//labelTimeEnd->SetRange(0, timeTotal);
	labelTimeStart->SetTime(0,0,0);

	int hour = 0;
	int minute = 0;
	int second = 0;
	GetTimeToHourMinuteSecond(timeTotal, hour, minute, second);
	labelTimeEnd->SetTime(hour, minute, second);


	wxColour bgColor = labelTimeStart->GetParent()->GetBackgroundColour();
	sliderVideoPosition = new CSliderVideoSelection(labelTimeStart->GetParent(), wxID_ANY, this, theme);
	sliderVideoPosition->SetPosition(slVideo->GetPosition());
	wxSize size = wxSize(400, theme.GetHeight());
	sliderVideoPosition->SetSize(size);
	sliderVideoPosition->SetTotalSecondTime(timeTotal);
	sliderVideoPosition->SetBackgroundColour(bgColor);
	slVideo->Show(false);

	Connect(XRCID("ID_STSTARTMOVIE"), wxEVT_TIME_CHANGED, (wxObjectEventFunction)&CompressionAudioVideoOption::OnSlideFromChange);
	Connect(XRCID("ID_STENDMOVIE"), wxEVT_TIME_CHANGED, (wxObjectEventFunction)&CompressionAudioVideoOption::OnSlideToChange);

	//Connect(XRCID("ID_STSTARTMOVIE"), wxEVT_TEXT, (wxObjectEventFunction)&CompressionAudioVideoOption::OnTextFromChange);
	//Connect(XRCID("ID_STENDMOVIE"), wxEVT_TEXT, (wxObjectEventFunction)&CompressionAudioVideoOption::OnTextToChange);

}

void CompressionAudioVideoOption::SetBitmap(const long &pos)
{
	bitmap->SetBitmap(CLibPicture::ConvertRegardsBitmapToWXImage(ffmpegTranscoding->GetVideoFrame(pos, 344, 200), true, false));
}

void CompressionAudioVideoOption::OnSlideFromChange(wxDateEvent& event)
{
	wxDateTime pos = event.GetDate();
	int hour = pos.GetHour();
	int minute = pos.GetMinute();
	int second = pos.GetSecond();
	long _timeTotal = hour * 3600 + minute * 60 + second;
	if (_timeTotal >= 0 && _timeTotal < sliderVideoPosition->GetTimeEnd())
	{
		sliderVideoPosition->SetStartTime(_timeTotal);
	}
	else
	{
		_timeTotal = sliderVideoPosition->GetTimeEnd() - 1;
		int hour = 0;
		int minute = 0;
		int second = 0;
		GetTimeToHourMinuteSecond(_timeTotal, hour, minute, second);
		labelTimeStart->SetTime(hour, minute, second);
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
		int hour = 0;
		int minute = 0;
		int second = 0;
		GetTimeToHourMinuteSecond(_timeTotal, hour, minute, second);
		labelTimeEnd->SetTime(hour, minute, second);
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
		//labelTimeStart->SetValue(value);
		//labelTimeStart->SetValue(ConvertSecondToTime(value));
		//timeStart->SetLabel(ConvertSecondToTime(value));
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
		//timeEnd->SetLabel(ConvertSecondToTime(value));
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

void CompressionAudioVideoOption::GetCompressionOption(CVideoOptionCompress * videoOptionCompress)
{
	
	if (videoOptionCompress != nullptr)
	{
		videoOptionCompress->videoEffectParameter->effectEnable = ckenablefilter->GetValue();
		videoOptionCompress->videoEffectParameter->denoiseEnable = ckdenoiseFilter->GetValue();
		videoOptionCompress->videoEffectParameter->denoisingLevel = denoiseFilter->GetValue();
		videoOptionCompress->videoEffectParameter->SharpenEnable = cksharpenFilter->GetValue();
		videoOptionCompress->videoEffectParameter->grayEnable = ckgrey->GetValue();
		videoOptionCompress->videoEffectParameter->sepiaEnable = cksepia->GetValue();
		videoOptionCompress->videoEffectParameter->filmgrainenable = cknoise->GetValue();
		videoOptionCompress->videoEffectParameter->contrast = contrastFilter->GetValue();
		videoOptionCompress->videoEffectParameter->brightness = lightFilter->GetValue();
		videoOptionCompress->videoEffectParameter->ColorBoostEnable = ckcolorBoost->GetValue();

		videoOptionCompress->videoEffectParameter->color_boost[0] = redFilter->GetValue();
		videoOptionCompress->videoEffectParameter->color_boost[1] = greenFilter->GetValue();
		videoOptionCompress->videoEffectParameter->color_boost[2] = blueFilter->GetValue();
		videoOptionCompress->videoEffectParameter->bandcEnable = cklightandcontrast->GetValue();

		videoOptionCompress->videoEffectParameter->sharpness = sharpenFilter->GetValue() / 10.0f;

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
		if (videoOptionCompress->encoder_profile == "")
			videoOptionCompress->encoder_profile = "main";
            
        if(cbVideoProfile != nullptr)
            videoOptionCompress->encoder_profile = cbVideoProfile->GetStringSelection();

		if (videoOptionCompress->videoCodec == "")
			videoOptionCompress->videoCodec = "H264";

		videoOptionCompress->videoPreset = "";// cbVideoPreset->GetStringSelection();
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
		int endTime = timeTotal;
		videoOptionCompress->endTime = (sliderVideoPosition->GetTimeEnd() != endTime || sliderVideoPosition->GetTimeStart() != 0) ? sliderVideoPosition->GetTimeEnd() : 0;
	}


}

void CompressionAudioVideoOption::OnbtnOkClick(wxCommandEvent& event)
{
	if (ckVideoBitRate->IsChecked() && txtBitRate->GetValue() == "")
	{
		wxMessageBox("Erreur bit rate value.", "Error", wxICON_ERROR);
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

