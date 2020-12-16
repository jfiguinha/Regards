#pragma once
#ifndef WX_PRECOMP
	//(*HeadersPCH(TiffOption)
	#include <wx/checklst.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif

#include <wx/spinctrl.h>
//(*Headers(TiffOption)
//*)

class CImageLoadingFormat;
class CVideoOptionCompress;
class CFFmpegTranscoding;
class CRegardsBitmap;
class CSliderVideoSelection;

class CompressionAudioVideoOption: public wxDialog
{
	public:

		CompressionAudioVideoOption(wxWindow* parent, const wxString &videoFilename);
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

		wxStaticBitmap * bitmap;
		wxSpinCtrl * labelTimeStart;
		wxSpinCtrl * labelTimeEnd;
		wxSlider * slVideo;
	protected:

		//(*Identifiers(TiffOption)
		//*)

	private:

		wxString ConvertSecondToTime(int64_t sec);
		//void OnPaint(wxPaintEvent &event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnbtnOkClick(wxCommandEvent& event);
		void OnbtnCheckAudioBitrateClick(wxCommandEvent& event);
		void OnbtnCheckAudioQualityClick(wxCommandEvent& event);
		void OnbtnCheckVideoQualityClick(wxCommandEvent& event);
		void OnbtnCheckVideoBitrateClick(wxCommandEvent& event);
		void OnSetVideoDuration(wxCommandEvent& event);
		void OnVideoSliderChange(wxCommandEvent& event);

		void OnSlideFromChange(wxSpinEvent& event);
		void OnSlideToChange(wxSpinEvent& event);

		void OnTextFromChange(wxCommandEvent& event);
		void OnTextToChange(wxCommandEvent& event);


		//void OnVideoSliderChange(wxScrollEvent& event);
		//*)
		double timeTotal;
        bool isOk;
		wxString videoFilename;
		CFFmpegTranscoding * ffmpegTranscoding;
		CRegardsBitmap * bitmapDisplay;
		CSliderVideoSelection * sliderVideoPosition;
		int ret = 0;
		wxImage scale;
		bool skipEvent = false;
		DECLARE_EVENT_TABLE()
};
