#pragma once
#ifdef WIN32

#ifndef WX_PRECOMP
	//(*HeadersPCH(TiffOption)
	#include <wx/checklst.h>
	#include <wx/button.h>
	#include <wx/dialog.h>

	//*)
#endif
//(*Headers(TiffOption)
//*)

class CImageLoadingFormat;
class CVideoOptionCompress;

class CompressionAudioVideoOption: public wxDialog
{
	public:

		CompressionAudioVideoOption(wxWindow* parent);
		virtual ~CompressionAudioVideoOption();
		void GetCompressionOption(CVideoOptionCompress * videoOptionCompress);

		wxButton* btnCancel;
		wxButton* btnOk;
		//wxPanel * panel;
		bool IsOk();

		wxCheckBox * ckAudioBitRate;
		wxComboBox * cbAudioBitRate;

		wxCheckBox * ckAudioQuality;
		wxComboBox * cbAudioQuality;

		wxCheckBox * ckVideoHardware; 
		wxComboBox * cbAudioCodec;

		
		wxComboBox * cbVideoCodec;
		wxComboBox * cbVideoPreset;
		wxRadioBox * rbQuality;

		wxCheckBox * ckVideoQuality;
		wxSlider * slCompression;

		wxCheckBox * ckVideoBitRate;
		wxTextCtrl * txtBitRate;

		wxComboBox * cbVideoProfile;

	protected:

		//(*Identifiers(TiffOption)
		//*)

	private:
		//void OnPaint(wxPaintEvent &event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnbtnOkClick(wxCommandEvent& event);
		void OnbtnCheckAudioBitrateClick(wxCommandEvent& event);
		void OnbtnCheckAudioQualityClick(wxCommandEvent& event);
		void OnbtnCheckVideoQualityClick(wxCommandEvent& event);
		void OnbtnCheckVideoBitrateClick(wxCommandEvent& event);
		//*)
        bool isOk;
		DECLARE_EVENT_TABLE()
};

#endif