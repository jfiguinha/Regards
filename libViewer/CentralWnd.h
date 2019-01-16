#pragma once
#include <ToolbarInterface.h>
#include <Photos.h>
#include <Pane.h>
#include "ThumbnailViewerVideo.h"
#include "EffectVideoParameter.h"
#include "PreviewThumbnailSplitter.h"
#include "ViewerParam.h"
#include "ThumbnailVideoPanel.h"
#include "ImageList.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
class CImageLoadingFormat;
namespace Regards
{
	namespace Viewer
	{

		class CCentralWnd : public wxWindow, public CPaneInterface
		{
		public:
			CCentralWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, CImageList * imageList);
			~CCentralWnd();

			void SetVideo(const wxString &path);
            bool SetAnimation(const wxString &filename);
			bool SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail);
			void SetNumElement(const int &numElement, const bool &move = true);
            void StartLoadingPicture(const int &numElement);
            void StopLoadingPicture();

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
			void ShowListFace();

			void Resize();
			void ShowFilter();
            wxString GetSqlRequest();

			void StartAnimation();
			void StopAnimation();
			void AnimationPictureNext();
			void AnimationPicturePrevious();

		private:
			void LoadAnimationBitmap(const int &numFrame);
			void OnSize(wxSizeEvent& event);
            void AnimationSetPosition(wxCommandEvent& event);
            void StartTimerAnimation(wxCommandEvent& event);
			void RedrawBarPos();
			wxRect GetWindowRect();
			vector<CImageVideoThumbnail *> listThumbnail;
			CThumbnailVideoPanel * thumbnailVideoPanel;
			CPreviewThumbnailSplitter * previewThumbnailSplitter;
			
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
