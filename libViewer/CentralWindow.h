#pragma once
#include <Photos.h>
#include <WindowMain.h>
using namespace std;
using namespace Regards::Window;
//using namespace Regards::Control;
class CImageLoadingFormat;
class CPictureElement;
class CImageVideoThumbnail;

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
	using namespace Regards::Internet;
	
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

			wxWindow * mainWindow;
			wxString picture;
			bool isVisible;
			bool isThumbnail;
			thread * myThread;
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

			CImageLoadingFormat * bitmap;
			bool isThumbnail;
			thread * myThread;
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

		class CCentralWindow : public CWindowMain
		{
		public:
			CCentralWindow(wxWindow* parent, wxWindowID id, const CThemeSplitter & theme, CImageList * imageList, const bool &horizontal = true);
			~CCentralWindow();

			void UpdateScreenRatio();
			void FullscreenMode();
			void ScreenMode();
			void HideToolbar();
			void ShowToolbar();
			void AnimationPictureNext();
			void AnimationPicturePrevious();
			void SetPosition(const long& timePosition);
			void StopAnimation();
			void StartAnimation();
			int RefreshPicture(const wxString &filename, const int &numElement, const bool &first = false);
			int LoadPicture(const wxString &filename, const int &numElement, const bool &first = false);
			void OnEndThumbnail();
            bool IsVideo();
			void SaveParameter();

		private:

			
			void OnLoadPicture(wxCommandEvent& event);
			bool GetProcessEnd();
            //void OnShowWindow(wxShowEvent& event);
			void LoadPictureInThread(CPictureElement * pictureElement);
			void LoadingPicture(const wxString &filenameToShow, const int &numElement);
			void EndPictureThread(wxCommandEvent& event);
			void StartLoadingPicture(const int &numElement);
			void OnVideoEnd(wxCommandEvent& event);
			virtual void Resize();
           void OnVideoStart(wxCommandEvent& event);
			void SetListeFile(wxCommandEvent& event);
			void ChangeTypeAffichage(wxCommandEvent& event);
			void SetMode(wxCommandEvent& event);
			void OnShowPicture(wxCommandEvent& event);
			void SetVideoPos(wxCommandEvent& event);
			void OnTimerAnimation(wxTimerEvent& event);
			void SetPicture(CImageLoadingFormat * bitmap, const bool &isThumbnail);
			void StopLoadingPicture();

			bool SetAnimation(const wxString &filename);

            
			CPanelPhotoWnd * panelPhotoWnd;
			CMainParam * viewerconfig;
			CListPicture * listPicture;
			CPanelWithClickToolbar * panelInfosClick;

			//Face List
#ifndef __NOFACE_DETECTION__
			CListFace * listFace;
#endif

            bool loadPicture = false;
            wxString filename;
            int numElement;

			CScrollbarWnd * scrollVideoWindow;
			CThumbnailViewerVideo * thumbnailVideo;

			//CPreviewThumbnailSplitter * previewThumbnailSplitter;
			CPanelInfosWnd * panelInfosWindow;

			//Preview
			CPreviewWnd * previewWindow;

			//Thumbnail Picture
			CScrollbarWnd * scrollPictureWindow;
			CThumbnailViewerPicture * thumbnailPicture;

			//Window List
			static void LoadingNewPicture(CThreadPictureData * pictureData);
			void AnimationSetPosition(wxCommandEvent& event);
			void LoadAnimationBitmap(const wxString &filename, const int &numFrame);
			bool SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail);
			bool SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation);
			void SetPanelInfos(const bool &isThumbnail);
			void SetVideo(const wxString &path, const bool &first);
			void SetModeViewer();

			PhotosVector photoVector;

			CWindowManager * windowManager;
			int windowMode;
			int oldWindowMode;
			wxTimer * animationTimer;
			int animationPosition;
			vector<CImageVideoThumbnail *> videoThumbnail;
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
			bool isThumbnail;
			bool videoStart;
            bool stopVideo;
            bool init = false;
			bool windowInit = true;

		};
	}
}