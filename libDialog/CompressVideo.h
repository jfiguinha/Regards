#pragma once
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

class CompressVideo: public wxDialog
{
	public:

		CompressVideo(wxWindow* parent);
		virtual ~CompressVideo();

		wxButton* btnCancel;
		wxGauge * ggProgress;
		wxStaticBitmap * bitmap;
		wxStaticText * labelProgression;
		wxStaticText * labelFrame;
		wxStaticText * labelTime;
		wxStaticText * labelTimeMissing;
		//wxPanel * panel;
		bool IsOk();
		void SetPos(const int &max, const int &pos);
		void SetBitmap(wxImage * bmp);
		void SetTextProgression(const wxString &texte, const int &type = 0);
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

		//*)
        bool isOk;
		wxImage scale;
		std::mutex muBitmap;
		DECLARE_EVENT_TABLE()
};
