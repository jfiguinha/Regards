#pragma once
#include <Splitter.h>
#include <ScrollbarWnd.h>
#include <PaneInterface.h>
#include <ClickToolbar.h>
#include <ToolbarInterface.h>
#include <Photos.h>
#include <Pane.h>
#include "ThumbnailBitmap.h"
#include "ThumbnailViewerVideo.h"
#include "EffectVideoParameter.h"
#ifdef VIEWER
#include "FilterPreviewSplitter.h"
#else
#include "PreviewInfosWnd.h"
#endif
#include "ViewerParam.h"
#ifdef __APPLE__
#include <AppleSelectFile.h>
#endif
#include <vector>
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;

namespace Regards
{
	namespace Viewer
	{

		class CCentralWnd : public wxWindow, public CPaneInterface, public CToolbarInterface
		{
		public:
			CCentralWnd(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, CVideoEffectParameter * videoEffectParameter);
			~CCentralWnd();

            void SetListeFile(const vector<wxString> listFichier);
			void SetVideo(const wxString &path);
            bool SetAnimation(const wxString &filename);
			bool SetBitmap(CRegardsBitmap * bitmap, const bool &isThumbnail);
			void SetNumElement(const int &numElement);
            void StartLoadingPicture(const int &numElement);
            void StopLoadingPicture();

            void UpdateScreenRatio();
			void ShowVideoThumbnail();
			void ShowPanelVideoThumbnail();
			void HidePanelVideoThumbnail();
			bool IsPanelVideoThumbnailVisible();

			void ShowPanelThumbnail();
			void HidePanelThumbnail();
			bool IsPanelThumbnailVisible();

			void ShowPanelInfos();
			void HidePanelInfos();
			bool IsPanelInfosVisible();

			void ClosePane(const int &id);

			void SetEffect(const bool &effect);

			void FullscreenMode();
			void ScreenMode();

			void SetDiaporamaMode();
			void SetNormalMode();

			void SetVideoPosition(const int64_t &videoTime);
			void ShowToolbar();
			void HideToolbar();

			void Resize();
            void RefreshThumbnail();
#ifdef VIEWER
			void ShowFilter();
			void UpdateCriteria(const int64_t & idFolder);
            wxString GetSqlRequest();
            void InitSaveParameter();
            void UpdateCriteria();
            void RefreshFilter();
#endif
		protected:
			void ClickShowButton(const int &id);

		private:

			void OnSize(wxSizeEvent& event);
			void RedrawBarPos();
			wxRect GetWindowRect();

			CPane * paneThumbnail = nullptr;
			CScrollbarWnd * scrollThumbnail = nullptr;
			CThumbnailBitmap * thumbnail = nullptr;
			CClickToolbar * clickThumbnail = nullptr;

			CPane * paneThumbnailVideo = nullptr;
			CScrollbarWnd * scrollThumbnailVideo = nullptr;
			CThumbnailViewerVideo * thumbnailVideo = nullptr;
			CClickToolbar * clickThumbnailVideo = nullptr;
#ifdef VIEWER
			CFilterPreviewSplitter * filterPreviewSplitter = nullptr;
#else
			CPreviewInfosWnd * previewInfosWnd = nullptr;
#endif		
			CViewerParam * viewerParam = nullptr;

			bool isVideo = false;
			bool isFullscreen = false;
			wxString filename = L"";
			int width = 0;
			int height = 0;
			bool isDiaporama = false;
            bool showToolbar = false;
		};


	}
}
