#pragma once
#include <Splitter.h>
#include "CategoryFolderWindow.h"
#include "PreviewInfosWnd.h"
#include "ViewerParam.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;

#define CATALOG_FILTER 2

namespace Regards
{
	namespace Viewer
	{
		class CFilterPreviewSplitter :public CSplitter, public CPaneInterface, public CToolbarInterface
		{
		public:
			CFilterPreviewSplitter(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, CVideoEffectParameter * videoEffectParameter, const bool &horizontal = true);
			~CFilterPreviewSplitter();
			void OnHidePane(wxCommandEvent& event);
			void ShowToolbar();
			void HideToolbar();
			void SetDiaporamaMode();
			void SetNormalMode();
			void UpdateScreenRatio();
			void SetEffect(const bool &effect);
			bool IsPanelInfosVisible();
			void HidePanelInfos();
			void ShowPanelInfos();
			void SetVideo(const wxString &path);
			void StartLoadingPicture();
			bool SetAnimation(const wxString &filename);
			bool SetBitmap(CRegardsBitmap * bitmap, const bool &isThumbnail);
			void FullscreenMode();
			void ScreenMode();
			void ClickShowButton(const int &id);
			void ClosePane(const int &id);
			void UpdateCriteria(const int64_t & idFolder);
            wxString GetSqlRequest();
            void InitSaveParameter();
            void UpdateInfos();
            void RefreshFilter();
            
		private:
			
			void ShowWindow(const bool & showInfos);

			int posBarInfos = 0;
			CPane * paneFilter = nullptr;
			CClickToolbar * clickInfoToolbar = nullptr;
			CCategoryFolderWindow * categoryFolderWnd = nullptr;
			CPreviewInfosWnd * previewInfosWnd = nullptr;
			CViewerParam * viewerconfig = nullptr;

			bool fullscreen = false;
			bool paneFilterShow = true;
			bool clickToobarShow = false;
		};
	}
}