#ifndef CONFIGREGARDS_H
#define CONFIGREGARDS_H

#ifndef WX_PRECOMP
	//(*HeadersPCH(ConfigRegards)
	#include <wx/stattext.h>
	#include <wx/radiobox.h>
	#include <wx/statbox.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	//*)
#endif
//(*Headers(ConfigRegards)
#include <wx/spinctrl.h>
//*)

class ConfigRegards: public wxDialog
{
	public:

		ConfigRegards(wxWindow* parent);
		virtual ~ConfigRegards();

		//(*Declarations(ConfigRegards)
		wxButton* btOk;
		wxRadioBox* rbDiaporamaTransitionEffect;
		wxRadioBox* rbFullscreen;
		wxStaticBox* sbDiaporama;
		wxSpinCtrl* scTime;
		wxRadioBox* rdPreviewRender;
		wxRadioBox* rbTransitionEffect;
		wxRadioBox* rbRenderVideo;
		wxRadioBox* rbEffectLibrary;
		wxButton* btCancel;
		wxStaticText* stTime;
		wxStaticBox* sbThumbnail;
		wxRadioBox* rbThumbnailQuality;
		wxRadioBox* rbThumbnailCache;
		//*)

	protected:

		//(*Identifiers(ConfigRegards)
		//*)

	private:

		//(*Handlers(ConfigRegards)
		void Init();
		void OnbtnOkClick(wxCommandEvent& event);
		void OnBtnCancelClick(wxCommandEvent& event);
		//*)
		bool isOk = false;
		DECLARE_EVENT_TABLE()
};

#endif
