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

class CompressVideo: public wxDialog
{
	public:

		CompressVideo(wxWindow* parent);
		virtual ~CompressVideo();

		wxButton* btnCancel;
		wxGauge * ggProgress;
		wxStaticBitmap * bitmap;
		wxStaticText * labelProgression;

		bool IsOk();
		void SetPos(const int &max, const int &pos);
		void SetBitmap(CImageLoadingFormat * loadingFormat);
		void SetBitmap(wxCommandEvent& event);
		void SetTextProgression(const wxString &texte);
	protected:

		//(*Identifiers(TiffOption)
		//*)

	private:

		void OnbtnCancelClick(wxCommandEvent& event);
		//*)
        bool isOk;
		wxImage scale;
		DECLARE_EVENT_TABLE()
};

#endif