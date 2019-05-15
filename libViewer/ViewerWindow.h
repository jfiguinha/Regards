#pragma once
#include <ToolbarInterface.h>
#include <Photos.h>
#include <WindowMain.h>
#include <PanelWithClickToolbar.h>
#include "PreviewInfosWnd.h"
#include "ThumbnailViewerVideo.h"
#include "ThumbnailViewerPicture.h"
#include "EffectVideoParameter.h"
#include "ViewerParam.h"
#include "ImageList.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
class CImageLoadingFormat;
namespace Regards
{
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

		class CViewerWindow : public CWindowMain, public CPaneInterface
		{
		public:
			CViewerWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface);
			~CViewerWindow();

			void SetListeFile(PhotosVector * photoVector);
			void SetVideo(const wxString &path);
            bool SetAnimation(const wxString &filename);
			bool SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail);
			void SetNumElement(const int &numElement, const bool &move = true);
            void StartLoadingPicture(const int &numElement);
            void StopLoadingPicture();
			void ClosePane(const int& id) {};
			void RefreshPane(const int& id) {};

            void UpdateScreenRatio();

			bool IsPanelThumbnailVisible();
			bool IsPanelInfosVisible();

			void SetEffect(const bool &effect);

			void FullscreenMode();
			void ScreenMode();
			
			void SetDiaporamaMode();
			void SetNormalMode();

			void SetVideoPosition(const int64_t &videoTime);
			void SetPosition(const long &timePosition);
			void ShowToolbar();
			void HideToolbar();
			void ShowPanelVideoThumbnail();
            void HidePanel();
			void Resize();

			void AnimationPictureNext();
			void AnimationPicturePrevious();

			void StartAnimation();
			void StopAnimation();

			bool GetProcessEnd();

		private:

			//Picture Loading
			void SetPicture(CImageLoadingFormat * bitmap, const bool &isThumbnail);
			void LoadPictureInThread(const wxString &filename, const int &numElement, const bool &load = false);
			static void LoadingNewPicture(CThreadPictureData * pictureData);
			void LoadingPicture(const wxString &filenameToShow);
			void OnLoadPicture(wxCommandEvent& event);
			void OnShowPicture(wxCommandEvent& event);
			void EndPictureThread(wxCommandEvent& event);

			void OnRefresh(wxCommandEvent& event);
			void OnResize(wxCommandEvent& event);
			void LoadAnimationBitmap(const int &numFrame);
			void OnSize(wxSizeEvent& event);
            void AnimationSetPosition(wxCommandEvent& event);
            void StartTimerAnimation(wxCommandEvent& event);
			void SetVideoPos(wxCommandEvent& event);

			void RedrawBarPos();
			wxRect GetWindowRect();
			vector<CImageVideoThumbnail *> listThumbnail;
			
			//Thumbnail Video
			CPanelWithClickToolbar * panelVideo;
			CScrollbarWnd * scrollVideoWindow;
			CThumbnailViewerVideo * thumbnailVideo;

			//CPreviewThumbnailSplitter * previewThumbnailSplitter;
			CPreviewInfosWnd * previewInfosWnd;
			
			//Thumbnail Picture
			CPanelWithClickToolbar * panelPicture;
			CScrollbarWnd * scrollPictureWindow;
			CThumbnailViewerPicture * thumbnailPicture;

			void OnTimerAnimation(wxTimerEvent& event);

			CViewerParam * viewerParam;
			bool checkValidity;
			bool isVideo;
			bool isAnimation;
			bool isPicture;
			bool isFullscreen;
			bool processLoadPicture;
			wxString filename;
			int width;
			int height;

			bool isDiaporama;
            bool showToolbar;
			wxTimer * animationTimer;
			int animationPosition;
		};


	}
}
