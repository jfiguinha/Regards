#pragma once

#ifndef WX_PRECOMP
	//(*HeadersPCH(CopyFileDlg)
	#include <wx/stattext.h>
	#include <wx/button.h>
	#include <wx/dialog.h>
	#include <wx/gauge.h>
	//*)
#endif
//(*Headers(CopyFileDlg)
//*)

#include <ThumbnailData.h>
#include <libPicture.h>
#include "ExportFile.h"
using namespace std;

class CPictureCategorieLoadData;
class CPictureCategorie;

class CCategoryDetectionDlg : public wxDialog
{
	public:

		CCategoryDetectionDlg(wxWindow* parent);
		virtual ~CCategoryDetectionDlg();

		//(*Declarations(CopyFileDlg)
		wxStaticText* StaticText2;
		wxButton* btOK;
		wxGauge* Gauge1;
		wxStaticText* StaticText1;
		//*)

	protected:

		//(*Identifiers(CopyFileDlg)
		//*)

	private:

		//(*Handlers(CopyFileDlg)
		void OnInit(wxInitDialogEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnIdle(wxIdleEvent& evt);
		void OnSize(wxSizeEvent& evt);
		void OnCategoryAdd(wxCommandEvent& event);
		//*)
		static void CategoryRecognition(void * param);

		short progress;
		int width;
		int height;
		bool start;
		int mode;
		int nbProcessFacePhoto;
		bool pushCloseButton;

		CPictureCategorieLoadData * categorieData;
		

		DECLARE_EVENT_TABLE()
};
