#pragma once
#include <StatusBarInterface.h>
#include "wx/print.h"
#include <wx/fswatcher.h>
#include <MyFrameIntro.h>
#include <MainInterface.h>

using namespace Regards::Introduction;

class CImageLoadingFormat;
class CScannerFrame;

namespace Regards
{
	namespace Viewer
	{
		// constants:
		enum
		{
			ID_Hello = 1,
			ID_Folder = 2,
			ID_Configuration = 3,
			ID_OpenCL = 4,
			ID_SIZEICONLESS = 5,
			ID_SIZEICONMORE = 6,
			ID_ERASEDATABASE = 7,
			ID_THUMBNAILRIGHT = 8,
			ID_THUMBNAILBOTTOM = 9,
			ID_FACEDETECTION = 10,
			ID_INTERPOLATIONFILTER = 11,
			ID_EXPORT = 12,
			ID_SCANNER = 13,
			ID_VIDEO = 1018,
			ID_AUDIO = 1019,
			ID_SUBTITLE = 1020,
			WXSCAN_PAGE,
			WXPRINT_PAGE_SETUP,
			WXPRINT_PAGE_SETUP_PS,
#ifdef __WXMAC__
			WXPRINT_PAGE_MARGINS,
#endif
		};

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
			void PrintImagePreview(CRegardsBitmap * imageToPrint);
			void Exit();
			void ShowViewer(){};
            void OpenFile(const wxString &filename);
            static bool GetViewerMode();
            static void SetViewerMode(const bool &viewerMode);
            bool AddFSEntry(const wxString& dirPath);
            bool RemoveFSEntry(const wxString& dirPath);
            
		private:
			void OnExport(wxCommandEvent& event);
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
			void OnScanner(wxCommandEvent& event);
			void OnExit(wxCommandEvent& event);
			void OnPageSetup(wxCommandEvent& event);
			void OnFaceDetection(wxCommandEvent& event);
			void OnCategoryDetection(wxCommandEvent& event);
			void ShowOpenCLConfiguration(const bool &showRestart);
            void OnFileSystemModified(wxFileSystemWatcherEvent& event);
			void HideScanner(wxCommandEvent& event);

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
			CScannerFrame * frameScanner;
			//CImageLoadingFormat * picture;
            wxFileSystemWatcher * m_watcher;
		};
	}
}
