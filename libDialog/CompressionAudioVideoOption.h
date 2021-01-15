#pragma once
#ifndef WX_PRECOMP
	//(*HeadersPCH(TiffOption)
	#include <wx/checklst.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif

#include <wx/timectrl.h>
#include <wx/dateevt.h>
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;
//(*Headers(TiffOption)
//*)
class CPreviewDlg;
class CImageLoadingFormat;
class CVideoOptionCompress;
class CThumbnailVideo;
class CRegardsBitmap;
class CSliderVideoSelection;
class CVideoEffectParameter;

class CompressionAudioVideoOption: public wxDialog
{
	public:

		CompressionAudioVideoOption(wxWindow* parent, const wxString &videoFilename, COpenCLEngine * openCLEngine);
		virtual ~CompressionAudioVideoOption();
		void GetCompressionOption(CVideoOptionCompress * videoOptionCompress);

		wxButton* btnCancel;
		wxButton* btnOk;
		wxButton* btnPreview;
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
		wxTimePickerCtrl  * labelTimeStart;
		wxTimePickerCtrl  * labelTimeEnd;
		wxSlider * slVideo;

		wxRadioBox * rbAudioDirectCopy;
		wxRadioBox * rbVideoDirectCopy;

		//Filter event
		wxCheckBox * ckdenoiseFilter;
		wxSlider * denoiseFilter;
		wxCheckBox * cksharpenFilter;
		wxSlider * sharpenFilter;
		wxCheckBox * cklightandcontrast;
		wxSlider * contrastFilter;
		wxSlider * lightFilter;
		wxCheckBox * ckcolorBoost;
		wxSlider * redFilter;
		wxSlider * greenFilter;
		wxSlider * blueFilter;
		wxCheckBox * ckgrey;
		wxCheckBox * cksepia;
		wxCheckBox * cknoise;
		wxCheckBox * ckenablefilter;
	protected:

		//(*Identifiers(TiffOption)
		//*)

	private:

		//wxImage ApplyFilter(CRegardsBitmap * bitmap, CVideoEffectParameter * videoEffectParameter);
		wxString ConvertSecondToTime(int64_t sec);
		//void OnPaint(wxPaintEvent &event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnbtnOkClick(wxCommandEvent& event);
		void OnbtnPreviewClick(wxCommandEvent& event);
		void OnbtnCheckAudioBitrateClick(wxCommandEvent& event);
		void OnbtnCheckAudioQualityClick(wxCommandEvent& event);
		void OnbtnCheckVideoQualityClick(wxCommandEvent& event);
		void OnbtnCheckVideoBitrateClick(wxCommandEvent& event);
		void OnSetVideoDuration(wxCommandEvent& event);
		void OnVideoSliderChange(wxCommandEvent& event);

		void OnSlideFromChange(wxDateEvent& event);
		void OnSlideToChange(wxDateEvent& event);
		void SetBitmap(const long &pos);

		//void OnVideoSliderChange(wxScrollEvent& event);
		//*)
		double timeTotal;
        bool isOk;
		wxString videoFilename;
		CThumbnailVideo * ffmpegTranscoding;
		CPreviewDlg * previewDlg;
		CSliderVideoSelection * sliderVideoPosition;
		CVideoEffectParameter * videoEffectParameter;
		int ret = 0;
		wxImage scale;
		bool skipEvent = false;
		DECLARE_EVENT_TABLE()
};
