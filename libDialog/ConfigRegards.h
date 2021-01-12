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
		wxStaticBox* sbDiaporama;
		wxSpinCtrl* scTime;
        wxSpinCtrl* scProcessExif;
        wxSpinCtrl* scProcessThumbnail;
		wxRadioBox* rdPreviewRender;
		wxRadioBox* rdDxva2Render;
		wxComboBox* rbTransitionEffect;
		wxRadioBox* rbRenderVideo;
		wxRadioBox* rbEffectLibrary;
		wxButton* btCancel;
		wxStaticText* stTime;
		wxStaticBox* sbThumbnail;
		wxRadioBox* rbThumbnailQuality;
		wxRadioBox* rbThumbnailCache;
		wxRadioBox* rbDatabaseInMemory;
		wxRadioBox* rbAutoRotate;
		wxRadioBox* rbContrastCorrection;
		wxComboBox* rbInterpolation;
		//*)

#ifdef WIN32
		wxCheckBox* ckDxva2Opengl;
		wxCheckBox* ckDxva2acc;
#endif

		bool IsOk();

	protected:

		//(*Identifiers(ConfigRegards)
		//*)

	private:

		//(*Handlers(ConfigRegards)
		void Init();
		void OnbtnOkClick(wxCommandEvent& event);
		void OnBtnCancelClick(wxCommandEvent& event);
		//*)
		bool isOk;
		DECLARE_EVENT_TABLE()
};

#endif
