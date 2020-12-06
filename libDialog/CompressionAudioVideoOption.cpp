#include <header.h>
#include "CompressionAudioVideoOption.h"
#include <window_id.h>
#include <VideoCompressOption.h>
#include <ImageLoadingFormat.h>
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

BEGIN_EVENT_TABLE(CompressionAudioVideoOption,wxDialog)
	//(*EventTable(CompressionAudioVideoOption)
	//*)
END_EVENT_TABLE()

CompressionAudioVideoOption::CompressionAudioVideoOption(wxWindow* parent)
{
    isOk = true;
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
	cbVideoPreset = (wxComboBox*)FindWindow(XRCID("ID_CBVIDEOPRESET"));
	cbVideoProfile = (wxComboBox*)FindWindow(XRCID("ID_CBVIDEOPROFILE"));
	rbQuality = (wxRadioBox*)FindWindow(XRCID("ID_RBQUALITY"));
	ckVideoQuality = (wxCheckBox*)FindWindow(XRCID("ID_CKVIDEOQUALITY"));
	slCompression = (wxSlider*)FindWindow(XRCID("ID_SLCOMPRESSIONQUALITY"));
	ckVideoBitRate = (wxCheckBox*)FindWindow(XRCID("ID_CKVIDEOBITRATE"));
	txtBitRate = (wxTextCtrl*)FindWindow(XRCID("ID_TXTBITRATE"));

	Connect(XRCID("ID_CKAUDIOBITRATE"), wxEVT_CHECKBOX, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckAudioBitrateClick);
	Connect(XRCID("ID_CKAUDIOQUALITY"), wxEVT_CHECKBOX, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckAudioQualityClick);
	Connect(XRCID("ID_CKVIDEOQUALITY"), wxEVT_CHECKBOX, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckVideoQualityClick);
	Connect(XRCID("ID_CKVIDEOBITRATE"), wxEVT_CHECKBOX, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCheckVideoBitrateClick);
	Connect(XRCID("ID_BTCANCEL"),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnCancelClick);
	Connect(XRCID("ID_BTOK"), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&CompressionAudioVideoOption::OnbtnOkClick);
}

CompressionAudioVideoOption::~CompressionAudioVideoOption()
{

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

		videoOptionCompress->videoPreset = cbVideoPreset->GetStringSelection();
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

