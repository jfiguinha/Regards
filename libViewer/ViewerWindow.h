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



		private:

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
