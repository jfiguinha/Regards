#pragma once

#define USE_PREVIEW_INTEGRATE

#ifndef WX_PRECOMP
	//(*HeadersPCH(TiffOption)
	#include <wx/checklst.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif

#include <wx/timectrl.h>
#include <wx/spinctrl.h>
#include <wx/dateevt.h>
#include <OpenCLEngine.h>
using namespace Regards::OpenCL;

namespace Regards
{
	namespace Control
	{
		class CShowPreview;
	}
}
using namespace Regards::Control;

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

		CompressionAudioVideoOption(wxWindow* parent, const wxString &videoFilename, const wxString &videoOutputFilename, COpenCLEngine * openCLEngine);
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

		wxCheckBox * ckVideoAutocontrast;
		wxCheckBox * ckVideoStabilization;
		wxSpinCtrl * spVideoStabilizationBuffer;

#ifdef USE_PREVIEW_INTEGRATE 
		wxPanel * panel;
#ifndef __APPLE__
		wxStaticBitmap * bitmapPreview;
#endif
#endif
		wxStaticBox * stPreviewPicture;
		void ChangeLabelPicture(const wxString &label);

	protected:

		//(*Identifiers(TiffOption)
		//*)

	private:

		//wxImage ApplyFilter(CRegardsBitmap * bitmap, CVideoEffectParameter * videoEffectParameter);
		wxString ConvertSecondToTime(int64_t sec);
		//void OnPaint(wxPaintEvent &event);
		void OnClosePreview(wxCommandEvent& event);
		void OnErrorCompression(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnbtnOkClick(wxCommandEvent& event);
		void OnbtnPreviewClick(wxCommandEvent& event);
		void OnbtnCheckAudioBitrateClick(wxCommandEvent& event);
		void OnbtnCheckAudioQualityClick(wxCommandEvent& event);
		void OnbtnCheckVideoQualityClick(wxCommandEvent& event);
		void OnbtnCheckVideoBitrateClick(wxCommandEvent& event);
		void OnbtnCheckFilterClick(wxCommandEvent& event);
		void OnbtnSliderFilterClick(wxScrollEvent& event);
		void OnSetVideoDuration(wxCommandEvent& event);
		void OnVideoSliderChange(wxCommandEvent& event);

		void OnVideoCodecSelect(wxCommandEvent& event);

		void OnSlideFromChange(wxDateEvent& event);
		void OnSlideToChange(wxDateEvent& event);
		void SetBitmap(const long &pos);

		//void OnVideoSliderChange(wxScrollEvent& event);
		//*)
		double timeTotal;
        bool isOk;
		wxString videoFilename;
		CThumbnailVideo * ffmpegTranscoding;
#ifndef USE_PREVIEW_INTEGRATE 
		CPreviewDlg * previewDlg;
#endif
		CSliderVideoSelection * sliderVideoPosition;
		CVideoEffectParameter * videoEffectParameter;
		//int ret = 0;
		wxImage scale;
		wxString extension;
		//bool skipEvent = false;
		//bool previewShow = false;
		CShowPreview * showBitmapWindow;
		DECLARE_EVENT_TABLE()
};
