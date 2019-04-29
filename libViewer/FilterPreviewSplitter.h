#pragma once
#include <Splitter.h>
#include "PanelPhotoWnd.h"
#include "PreviewInfosWnd.h"
#include "ViewerParam.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
class CImageLoadingFormat;
#define CATALOG_FILTER 2

namespace Regards
{
	namespace Viewer
	{
		class CFilterPreviewSplitter :public CSplitter, public CPaneInterface, public CToolbarInterface
		{
		public:
			CFilterPreviewSplitter(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, const bool &horizontal = true);
			~CFilterPreviewSplitter();


			void SetDiaporamaMode();
			void SetNormalMode();
			void UpdateScreenRatio();
			void SetEffect(const bool &effect);
			bool IsPanelInfosVisible();

			void SetVideo(const wxString &path);
			void StartLoadingPicture();
			bool SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation);
			void FullscreenMode();
			void ScreenMode();
			void ClickShowButton(const int &id);
			void ClosePane(const int &id);
			wxString GetSqlRequest();
            void HidePanel();
			
		private:

			void OnHidePane(wxCommandEvent& event);
			void ShowPanelPhotoList(wxCommandEvent& aEvent);
			void CloseWindow(wxCommandEvent& event);
			void ShowWindow(const bool & showInfos);

			int posBarInfos;
			CPane * panePhoto;
			CClickToolbar * clickInfoToolbar;
			CPanelPhotoWnd * panelPhotoWnd;
			CPreviewInfosWnd * previewInfosWnd;
			CViewerParam * viewerconfig ;

			bool fullscreen;
			bool paneFilterShow;
			bool clickToobarShow;
		};
	}
}