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
#include <videothumb.h>
#include "ShowPreview.h"



using namespace Regards::Control;

//(*Headers(TiffOption)

class CImageLoadingFormat;
class CVideoOptionCompress;
class CSliderVideoSelection;
class CVideoEffectParameter;


class CompressionAudioVideoOption : public wxDialog
{
public:
	CompressionAudioVideoOption();
	~CompressionAudioVideoOption() = default;
	void GetCompressionOption();
	void SetFile(const wxString& videoFilename, const wxString& videoOutputFilename);
	wxButton* btnCancel;
	wxButton* btnOk;
	wxButton* btnPreview;
	//wxPanel * panel;
	bool IsOk();
	bool IsCancel();

	wxCheckBox* ckAudioBitRate;
	wxComboBox* cbAudioBitRate;

	wxCheckBox* ckAudioQuality;
	wxComboBox* cbAudioQuality;

	wxCheckBox* ckVideoHardware;
	wxComboBox* cbVideoHardware;

	wxComboBox* cbAudioCodec;


	wxComboBox* cbVideoCodec;
	wxComboBox* cbVideoPreset;
	wxRadioBox* rbQuality;

	wxCheckBox* ckVideoQuality;
	wxSlider* slCompression;

	wxCheckBox* ckVideoBitRate;
	wxTextCtrl* txtBitRate;

	wxComboBox* cbVideoProfile;

	wxStaticBitmap* bitmap;
	wxTimePickerCtrl* labelTimeStart;
	wxTimePickerCtrl* labelTimeEnd;
	wxSlider* slVideo;

	//Filter event
	wxCheckBox* ckdenoiseFilter;
	wxSlider* denoiseFilter;
	wxCheckBox* cksharpenFilter;
	wxSlider* sharpenFilter;
	wxCheckBox* cklightandcontrast;
	wxSlider* contrastFilter;
	wxSlider* lightFilter;
	wxCheckBox* ckcolorBoost;
	wxSlider* redFilter;
	wxSlider* greenFilter;
	wxSlider* blueFilter;
	wxCheckBox* ckgrey;
	wxCheckBox* cksepia;
	wxCheckBox* cknoise;
	wxCheckBox* ckenablefilter;

	wxCheckBox* ckenableColorisation;
	wxCheckBox* ckenableRestore;

	wxRadioBox* rbAudioDirectCopy;
	wxRadioBox* rbVideoDirectCopy;

	wxCheckBox* ckVideoAutocontrast;
	wxCheckBox* ckVideoStabilization;
	wxSpinCtrl* spVideoStabilizationBuffer;

#ifdef USE_PREVIEW_INTEGRATE
	wxPanel* panel;
#ifndef __APPLE__
	wxStaticBitmap* bitmapPreview;
#endif
#endif
	wxStaticBox* stPreviewPicture;
	void ChangeLabelPicture(const wxString& label);
	CVideoOptionCompress * GetVideoCompressionPt();
protected:
	//(*Identifiers(TiffOption)
	//*)

private:
	//wxImage ApplyFilter(CRegardsBitmap * bitmap, CVideoEffectParameter * videoEffectParameter);
	wxString ConvertSecondToTime(int64_t sec);
	void SetBitmap(const long& pos);
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

	double timeTotal;
	bool isOk;
	wxString videoFilename;
	//int ret = 0;
	wxImage scale;
	wxString extension;
	bool isCancel = false;

	std::unique_ptr<Regards::Video::CVideoThumb>  ffmpegTranscoding = nullptr;
	std::unique_ptr<CSliderVideoSelection> sliderVideoPosition = nullptr;
	std::unique_ptr<CVideoEffectParameter> videoEffectParameter = nullptr;
	std::unique_ptr<CShowPreview> showBitmapWindow = nullptr;
	std::unique_ptr<CVideoOptionCompress> videoCompressOption;
	DECLARE_EVENT_TABLE()
};
