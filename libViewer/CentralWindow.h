#pragma once
#include <WindowMain.h>
using namespace std;
using namespace Regards::Window;
//using namespace Regards::Control;
class CImageLoadingFormat;
class CPictureElement;
class CImageVideoThumbnail;
class CFFmfc;
#define CATALOG_FILTER 2

namespace Regards
{
	namespace Window
	{
		class CPanelWithClickToolbar;
		class CScrollbarWnd;
		class CWindowManager;
	}

	namespace Internet
	{
		class CFileGeolocation;
	}

	using namespace Internet;

	namespace Viewer
	{
		class CThreadPictureData
		{
		public:
			CThreadPictureData()
			{
				mainWindow = nullptr;
				isVisible = false;
				myThread = nullptr;
				isThumbnail = false;
			}

			~CThreadPictureData()
			{
			}

			wxWindow* mainWindow;
			wxString picture;
			bool isVisible;
			bool isThumbnail;
			bool isDiaporama;
			int processLoadPicture;
			thread* myThread;
		};

		class CBitmapReturn
		{
		public:
			CBitmapReturn()
			{
				bitmap = nullptr;
				isThumbnail = false;
				myThread = nullptr;
			};

			CImageLoadingFormat* bitmap;
			bool isThumbnail;
			thread* myThread;
		};

		class CPanelPhotoWnd;
		class CListFace;
		class CListPicture;
		class CThumbnailViewerVideo;
		class CPanelInfosWnd;
		class CPreviewWnd;
		class CThumbnailViewerPicture;
		class CImageList;
		class CMainParam;
		class CLoadFile;

		class CCentralWindow : public CWindowMain
		{
		public:
			CCentralWindow(wxWindow* parent, wxWindowID id, const CThemeSplitter& theme, const bool& horizontal = true);
			~CCentralWindow() override;


			wxString ImageSuivante(const bool& loadPicture = true);
			wxString ImagePrecedente(const bool& loadPicture = true);
			wxString ImageFin(const bool& loadPicture = true);
			wxString ImageDebut(const bool& loadPicture = true);


			wxString GetFilename();
			int GetNbElement();

			void UpdateScreenRatio() override;
			bool FullscreenMode();
			bool ScreenMode();
			void HideToolbar();
			void ShowToolbar();
			void AnimationPictureNext();
			void AnimationPicturePrevious();
			void SetPosition(const long& timePosition);
			void SetListeFile(const wxString& filename);
			int LoadPicture(const wxString& filename, const bool& refresh = false);
			void OnEndThumbnail();
			bool IsVideo();
			void SaveParameter() override;
			bool IsCompatibleFullscreen();

			vector<wxString> GetFileList();
			void TransitionEnd();

			bool IsDiaporamaStart();

		private:
			int GetPhotoId(const wxString& filename);

			bool GetProcessEnd() override;

			void StartLoadingPicture();
			void OnVideoEnd(wxCommandEvent& event);
			void Resize() override;
			void OnVideoStart(wxCommandEvent& event);
			void OnAnimationStart(wxCommandEvent& event);
			void OnAnimationStop(wxCommandEvent& event);

			void StartDiaporamaMessage(wxCommandEvent& event);
			void StopAnimationEvent(wxCommandEvent& event);
			void OnTimerDiaporama(wxTimerEvent& event);

			void OnPicturePrevious(wxCommandEvent& event);
			void OnPictureNext(wxCommandEvent& event);
			void OnPictureFirst(wxCommandEvent& event);
			void OnPictureLast(wxCommandEvent& event);

			void StopDiaporama(wxCommandEvent& event);
			void StartDiaporama(wxCommandEvent& event);

			void OnVideoStop(wxCommandEvent& event);
			void ChangeTypeAffichage(wxCommandEvent& event);
			void SetMode(wxCommandEvent& event);
			void OnShowPicture(wxCommandEvent& event);
			void SetVideoPos(wxCommandEvent& event);

			void OnTimerAnimation(wxTimerEvent& event);
			void OnTimerResize(wxTimerEvent& event);
			void StopLoadingPicture();
			void StopAnimation();
			void StartAnimation();
			bool SetAnimation(const wxString& filename);


			CPanelPhotoWnd* panelPhotoWnd;
			CMainParam* viewerconfig;
			CListPicture* listPicture;
			CPanelWithClickToolbar* panelInfosClick;
			bool isNext = false;
			//Face List
#ifndef __NOFACE_DETECTION__
			CListFace* listFace;
#endif

			bool loadPicture = false;
			wxString filename;
			//int numElement;
			int faceDetection = 0;
			CScrollbarWnd* scrollVideoWindow;
			CThumbnailViewerVideo* thumbnailVideo;
			CPanelInfosWnd* panelInfosWindow;
			CPreviewWnd* previewWindow;
			CScrollbarWnd* scrollPictureWindow;
			CThumbnailViewerPicture* thumbnailPicture;

			//Window List
			static void LoadingNewPicture(CThreadPictureData* pictureData);
			void AnimationSetPosition(wxCommandEvent& event);
			void LoadAnimationBitmap(const wxString& filename, const int& numFrame);
			void SetPanelInfos(const bool& isThumbnail);
			void SetVideo(const wxString& path);
			void ShowPicture(CBitmapReturn* pictureData, const int& redraw);

			CWindowManager* windowManager;
			int windowMode;
			int oldWindowMode;
			wxTimer* animationTimer;
			wxTimer* resizeTimer;
			int animationPosition;
			vector<CImageVideoThumbnail*> videoThumbnail;
			bool processLoadPicture;
			int nbThumbnail;

			int oldAnimationPosition = -1;
			wxString oldFilename = L"";
			bool showToolbar;
			bool isFullscreen;
			bool isPicture;
			bool isAnimation;
			bool isVideo;
			bool isDiaporama;
			bool videoStart;
			bool stopVideo;
			bool init = false;
			bool windowInit = true;


			//Music Management
			void OnQuitAudio(wxCommandEvent& event);
			void OnStopAudio(wxCommandEvent& event);
			void StartMusic();
			void StopMusic();
			CFFmfc* ffmfc = nullptr;
			bool musicStop = true;
			bool ffmfcQuit = false;
			bool musicPause = false;
			int64_t musicPosition = 0;
			bool updateRightPanel = false;

			wxTimer* diaporamaTimer;

		};
	}
}
