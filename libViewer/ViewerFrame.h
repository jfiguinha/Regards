#pragma once
#include <StatusBarInterface.h>
#include <RegardsBitmap.h>
#include "wx/print.h"
#include <MyFrameIntro.h>
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
#include <MainInterface.h>
using namespace Regards::Introduction;

namespace Regards
{
	namespace Viewer
	{
		class CMainWindow;
		class CViewerParam;
		class CViewerTheme;

		class CViewerFrame : public wxFrame, public IStatusBarInterface
		{
		public:
			CViewerFrame(const wxString& title, const wxPoint& pos, const wxSize& size, IMainInterface * mainInterface, const wxString &fileToOpen = "");
			~CViewerFrame();
			void SetFolder(const wxString &lastFolder);
			void SetDatabase();
            void OpenFile(const wxString &filename);
			void SetText(const int &numPos, const wxString &libelle);
			void SetRangeProgressBar(const int &range);
			void SetPosProgressBar(const int &position);
			void SetWindowTitle(const wxString &libelle);
			void SetFullscreen();
			void SetScreen();
			void PrintPreview(CRegardsBitmap * imageToPrint);
			void Exit();
			void ShowViewer(){};
            static bool GetViewerMode();
            static void SetViewerMode(const bool &viewerMode);
		private:
            void OnHelp(wxCommandEvent& event);
            void OnIconSizeLess(wxCommandEvent& event);
            void OnIconSizeMore(wxCommandEvent& event);
			void OnClose(wxCloseEvent& event);
			void OnKeyDown(wxKeyEvent& event);
			void OnAbout(wxCommandEvent& event);
			void OnHello(wxCommandEvent& event);
			void OnConfiguration(wxCommandEvent& event);
			void OnConfigurationOpenCL(wxCommandEvent& event);
			void OnSelectFolder(wxCommandEvent& event);
			void OnExit(wxCommandEvent& event);
			void OnPageSetup(wxCommandEvent& event);
#ifdef __WXMAC__
			void OnPageMargins(wxCommandEvent& event);
#endif

			wxDECLARE_EVENT_TABLE();

			IMainInterface * mainInterface = nullptr;
			CViewerParam * viewerParam = nullptr;
			CViewerTheme * viewerTheme = nullptr;
			CMainWindow * mainWindow = nullptr;
			wxPreviewFrameModalityKind m_previewModality;
			bool fullscreen = false;
            static bool viewerMode;
		};
	}
}