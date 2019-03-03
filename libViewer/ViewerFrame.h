#pragma once
#include <StatusBarInterface.h>
#include "wx/print.h"
#include <MyFrameIntro.h>
#include <MainInterface.h>
using namespace Regards::Introduction;

class CRegardsBitmap;

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
			void PrintPreview(CRegardsBitmap * imageToPrint);
			void Exit();
			void ShowViewer(){};
            void OpenFile(const wxString &filename);
            static bool GetViewerMode();
            static void SetViewerMode(const bool &viewerMode);
		private:
            void OnHelp(wxCommandEvent& event);
            void OnIconSizeLess(wxCommandEvent& event);
            void OnIconSizeMore(wxCommandEvent& event);
			void OnThumbnailBottom(wxCommandEvent& event);
			void OnFacePertinence(wxCommandEvent& event);
			void OnThumbnailRight(wxCommandEvent& event);
			void OnKeyDown(wxKeyEvent& event);
			void OnAbout(wxCommandEvent& event);
			void OnHello(wxCommandEvent& event);
			void CheckAllProcessEnd(wxTimerEvent& event);
			void OnClose(wxCloseEvent& event);
			void OnConfiguration(wxCommandEvent& event);
			void OnEraseDatabase(wxCommandEvent& event);
			void OnOpenCLConfiguration(wxCommandEvent& event);
			void OnExit(wxCommandEvent& event);
			void OnPageSetup(wxCommandEvent& event);
			void BoxFilter(wxCommandEvent& event);
			void BilinearFilter(wxCommandEvent& event);
			void GaussianFilter(wxCommandEvent& event);
			void HammingFilter(wxCommandEvent& event);
			void CubicFilter(wxCommandEvent& event);
			void BlackmanFilter(wxCommandEvent& event);
			void QuadraticFilter(wxCommandEvent& event);
			void MitchellFilter(wxCommandEvent& event);
			void TriangleFilter(wxCommandEvent& event);
			void SincFilter(wxCommandEvent& event);
			void BesselFilter(wxCommandEvent& event);
			void BlackmanBesselFilter(wxCommandEvent& event);
			void BlackmanSincFilter(wxCommandEvent& event);
			void LanczosFilter(wxCommandEvent& event);
			void HermiteFilter(wxCommandEvent& event);
			void HanningFilter(wxCommandEvent& event);
			void CatromFilter(wxCommandEvent& event);
			void ShowOpenCLConfiguration(const bool &showRestart);

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

			void OnTimerLoadPicture(wxTimerEvent& event);
			wxTimer * loadPictureTimer;
			wxString filenameTimer;
			int nbTime;
			CRegardsBitmap * picture;
		};
	}
}
