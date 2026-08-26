#pragma once
#include <atomic>
#ifndef WX_PRECOMP
//(*HeadersPCH(TiffOption)
#include <wx/checklst.h>
#include <wx/button.h>
#include <wx/dialog.h>

//*)
#endif
//(*Headers(TiffOption)
//*)
;

class CompressVideo : public wxDialog
{
public:
	CompressVideo(wxWindow* parent, int rotation);
	~CompressVideo() override;

	wxButton* btnCancel;
	wxGauge* ggProgress;
	wxStaticBitmap* bitmap;
	wxStaticText* labelProgression;
	wxStaticText* labelFrame;
	wxStaticText* labelTime;
	wxStaticText* labelTimeMissing;
	//wxPanel * panel;
	bool IsOk();
	bool IsCancel();
	void SetPos(const int& max, const int& pos);
	void SetBitmap(cv::Mat& bmp);
	void SetTextProgression(const wxString& texte, const int& type = 0);

protected:
	//(*Identifiers(TiffOption)
	//*)

private:
	//void OnPaint(wxPaintEvent &event);
	void OnbtnCancelClick(wxCommandEvent& event);
	void OnSetValueProgressBar(wxCommandEvent& event);
	void OnSetValueMaxProgressBar(wxCommandEvent& event);
	void OnSetText(wxCommandEvent& event);
	void OnSetBitmap(wxCommandEvent& event);
	wxBitmap _localBmp;
	//*)
	// Written from the UI thread (OnbtnCancelClick) and read from the FFmpeg
	// encode worker thread (via IsOk()) now that EncodeFile runs on a
	// std::thread: must be atomic to avoid a data race / UB, and to
	// guarantee the worker actually observes a cancellation in time.
	std::atomic<bool> isOk = false;
	std::atomic<bool> isCancel = false;
	int rotation = 0;
	wxImage scale;
	std::mutex muBitmap;
	DECLARE_EVENT_TABLE()
};