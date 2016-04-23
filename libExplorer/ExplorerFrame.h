#pragma once
#include "wx/print.h"
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <StatusBarInterface.h>
#include <MyFrameIntro.h>
#include <RegardsBitmap.h>
#include <MainInterface.h>
using namespace Regards::Introduction;

namespace Regards
{
	namespace Explorer
	{
		class CMainWindow;
		class CExplorerParam;
		class CExplorerTheme;

		class CExplorerFrame : public wxFrame, public IStatusBarInterface
		{
		public:
			CExplorerFrame(const wxString& title, const wxPoint& pos, const wxSize& size, IMainInterface * mainInterface);
			~CExplorerFrame();
			void SetText(const int &numPos, const wxString &libelle);
			void SetRangeProgressBar(const int &range);
			void SetPosProgressBar(const int &position);
			void SetWindowTitle(const wxString &libelle);
			void SetFullscreen(){};
			void SetScreen(){};
			void PrintPreview(CRegardsBitmap * imageToPrint);
			void Exit();
			void ShowViewer();
			void OnKeyDown(wxKeyEvent& event);

		private:

            void OnIconSizeLess(wxCommandEvent& event);
            void OnIconSizeMore(wxCommandEvent& event);
			void OnExit(wxCommandEvent& event);
			void OnAbout(wxCommandEvent& event);
            void OnHelp(wxCommandEvent& event);
			void OnHello(wxCommandEvent& event);
			void OnConfiguration(wxCommandEvent& event);
			void OnConfigurationOpenCL(wxCommandEvent& event);
			void OnPageSetup(wxCommandEvent& event);
#if wxUSE_POSTSCRIPT
			void OnPageSetupPS(wxCommandEvent& event);
#endif
#ifdef __WXMAC__
			void OnPageMargins(wxCommandEvent& event);
#endif

			wxDECLARE_EVENT_TABLE();
			CMainWindow * mainWindow = nullptr;
			wxPreviewFrameModalityKind m_previewModality;
			CExplorerParam * explorerParam = nullptr;
			CExplorerTheme * explorerTheme = nullptr;
			IMainInterface * mainInterface = nullptr;
		};
	}
}

