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

#include "MultiThreadFiltre.h"
#include <thread>
using namespace std;

class CBm3dDlg: public wxDialog
{
	public:

		CBm3dDlg(wxWindow* parent, IMultiThreadFiltre * multiThreadProcess);
		virtual ~CBm3dDlg();
		void Start();
        bool IsProcessCancel();
		//(*Declarations(CopyFileDlg)
		wxStaticText* StaticText2;
		wxButton* btOK;
		wxGauge* Gauge1;
		wxStaticText* StaticText1;
		//*)

private:

		//(*Handlers(CopyFileDlg)
        void OnPaint(wxPaintEvent& event);
        void OnEndProcess(wxPaintEvent& event);
		void OnInit(wxInitDialogEvent& event);
		void OnButton1Click(wxCommandEvent& event);
		void OnIdle(wxIdleEvent& evt);
		void OnSize(wxSizeEvent& evt);
        void EndProcess(wxCommandEvent& event);
		//*)
        IMultiThreadFiltre * multiThreadProcess = nullptr;
        bool cancel = false;
        bool start = false;
        bool processCancel = false;
        bool processEnd = false;
        int i = 0;
        int nbtache = 0;
        int nbLoop = 0;
        thread process;
		DECLARE_EVENT_TABLE()
};
