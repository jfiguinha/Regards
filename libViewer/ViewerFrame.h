#pragma once
#include <StatusBarInterface.h>
#include "wx/print.h"
#include <wx/fswatcher.h>
#include <MyFrameIntro.h>
#include <MainInterface.h>
using namespace Regards::Introduction;

class CImageLoadingFormat;

namespace Regards
{
	namespace Viewer
	{
		class CMainWindow;
		class CWaitingWindow;
		class CViewerParam;
		class CViewerTheme;

		class CViewerFrame : public wxFrame, public IStatusBarInterface
		{
		public:
			CViewerFrame(const wxString& title, const wxPoint& pos, const wxSize& size, IMainInterface * mainInterface, const wxString &fileToOpen = "");
			~CViewerFrame();
			void SetText(const int &numPos, const wxString &libelle);
			void SetRangeProgressBar(const int &range);
			void SetPosProgressBar(const int &position);
			void SetWindowTitle(const wxString &libelle);
			void SetFullscreen();
			void SetScreen();
			void PrintPreview(CImageLoadingFormat * imageToPrint);
			void Exit();
			void ShowViewer(){};
            void OpenFile(const wxString &filename);
            static bool GetViewerMode();
            static void SetViewerMode(const bool &viewerMode);
            bool AddFSEntry(const wxString& dirPath);
            bool RemoveFSEntry(const wxString& dirPath);
            
		private:
			void OnPrint(wxCommandEvent& event);
            void OnHelp(wxCommandEvent& event);
            void OnIconSizeLess(wxCommandEvent& event);
            void OnIconSizeMore(wxCommandEvent& event);
			void OnFacePertinence(wxCommandEvent& event);
			void OnKeyDown(wxKeyEvent& event);
			void OnAbout(wxCommandEvent& event);
			void OnHello(wxCommandEvent& event);
			void CheckAllProcessEnd(wxTimerEvent& event);
			void OnClose(wxCloseEvent& event);
			void OnConfiguration(wxCommandEvent& event);
			void OnEraseDatabase(wxCommandEvent& event);
			void OnInterpolationFilter(wxCommandEvent& event);
			void OnOpenCLConfiguration(wxCommandEvent& event);
			void OnExit(wxCommandEvent& event);
			void OnScanPage(wxCommandEvent& event);
			void OnPageSetup(wxCommandEvent& event);
			void OnFaceDetection(wxCommandEvent& event);
			void OnCategoryDetection(wxCommandEvent& event);
			void ShowOpenCLConfiguration(const bool &showRestart);
            void OnFileSystemModified(wxFileSystemWatcherEvent& event);
			
#ifdef __WXMAC__
			void OnPageMargins(wxCommandEvent& event);
#endif

			wxDECLARE_EVENT_TABLE();


			IMainInterface * mainInterface;
			CViewerParam * viewerParam;
			CViewerTheme * viewerTheme;
			CMainWindow * mainWindow;
			wxTimer * exitTimer;
			CWaitingWindow * mainWindowWaiting;
			wxPreviewFrameModalityKind m_previewModality;
			bool fullscreen;
            static bool viewerMode;
            bool onExit;
            bool m_fsWatcher = true;
			void OnTimerLoadPicture(wxTimerEvent& event);
			wxTimer * loadPictureTimer;
			wxString filenameTimer;
			int nbTime;
			CImageLoadingFormat * picture;
            wxFileSystemWatcher * m_watcher;
		};
	}
}
