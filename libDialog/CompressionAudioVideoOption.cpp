#include <header.h>
#include "CompressionAudioVideoOption.h"
#include <window_id.h>
#include <VideoCompressOption.h>
#include <ImageLoadingFormat.h>
#include <ffmpeg_transcoding.h>
#include <RegardsBitmap.h>
#include <wx/filename.h>
#include <libPicture.h>
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

CompressionAudioVideoOption::CompressionAudioVideoOption(wxWindow* parent, const wxString &videoFilename)
{
    isOk = true;
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
	labelTimeStart  = (wxStaticText *)FindWindow(XRCID("ID_STSTARTMOVIE"));
	labelTimeEnd = (wxStaticText *)FindWindow(XRCID("ID_STENDMOVIE"));
	slVideo = (wxSlider*)FindWindow(XRCID("ID_SLVIDEO"));
	Connect(XRCID("ID_CKAUDIOBITRATE"), wxEVT_CHECKBOX, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckAudioBitrateClick);
	Connect(XRCID("ID_CKAUDIOQUALITY"), wxEVT_CHECKBOX, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckAudioQualityClick);
	Connect(XRCID("ID_CKVIDEOQUALITY"), wxEVT_CHECKBOX, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckVideoQualityClick);
	Connect(XRCID("ID_CKVIDEOBITRATE"), wxEVT_CHECKBOX, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckVideoBitrateClick);
	Connect(XRCID("ID_BTCANCEL"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCancelClick);
	Connect(XRCID("ID_BTOK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnOkClick);
	Connect(XRCID("ID_SLVIDEO"), wxEVT_SCROLL_CHANGED, (wxObjectEventFunction)&CompressionAudioVideoOption::OnVideoSliderChange);
	Connect(wxEVENT_SETVIDEODURATION, wxCommandEventHandler(CompressionAudioVideoOption::OnSetVideoDuration));

	bitmapDisplay = new CRegardsBitmap();
	ffmpegTranscoding = new CFFmpegTranscoding("");
	ret = ffmpegTranscoding->OpenVideoFile(videoFilename);
	ret = ffmpegTranscoding->GetFrameBitmapPosition(0, bitmapDisplay);
	wxImage * _wxImage = CLibPicture::ConvertRegardsBitmapToWXImage(bitmapDisplay);
	scale = _wxImage->Scale(344, 200).Mirror(false);
	bitmap->SetBitmap(scale);
	delete _wxImage;

	double timeTotal = ffmpegTranscoding->GetDuration();
	slVideo->SetMax(timeTotal);
	labelTimeEnd->SetLabel(ConvertSecondToTime(timeTotal));
}

void CompressionAudioVideoOption::OnVideoSliderChange(wxScrollEvent& event)
{
	long value = slVideo->GetValue();

	ret = ffmpegTranscoding->GetFrameBitmapPosition(value, bitmapDisplay);
	wxImage * _wxImage = CLibPicture::ConvertRegardsBitmapToWXImage(bitmapDisplay);
	scale = _wxImage->Scale(344, 200).Mirror(false);
	bitmap->SetBitmap(scale);
	delete _wxImage;

	labelTimeStart->SetLabel(ConvertSecondToTime(value));
}

CompressionAudioVideoOption::~CompressionAudioVideoOption()
{
	ffmpegTranscoding->CloseVideoFile();
	delete ffmpegTranscoding;
	delete bitmapDisplay;
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
	labelTimeEnd->SetLabel(ConvertSecondToTime(duration));
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

