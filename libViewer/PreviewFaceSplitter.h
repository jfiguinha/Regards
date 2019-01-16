#pragma once
#include <SplitterWithPanel.h>
#include "FilterPreviewSplitter.h"
#ifdef NORMAL
#include "ThumbnailFolder.h"
#else
#include "ListPicture.h"
#include "ListFace.h"
#endif
#include "ViewerParam.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
class CImageLoadingFormat;
namespace Regards
{
	namespace Viewer
	{

		class CPreviewFaceSplitter :public CSplitterWithPanel
		{
		public:
			CPreviewFaceSplitter(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, const CThemeToolbar & themeInfosToolbar);
			~CPreviewFaceSplitter();

			void SetEffect(const bool &effect);
			bool IsPanelInfosVisible();
			void SetVideo(const wxString &path);
			bool SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation);
			void FullscreenMode();
			bool IsPanelFaceVisible();
			void ScreenMode();
			void ShowListFace();
			void SetDiaporamaMode();
			void SetNormalMode();
			void SetActifItem(const int &numItem, const bool &move);
			void StartLoadingPicture(const int &numElement);
			void ShowFilter();
			wxString GetSqlRequest();
            void HidePanel();
		private:
			int posBarInfos;
			CFilterPreviewSplitter * filterPreviewSplitter;
			CListFace * listFace;
			CViewerParam * viewerconfig;
			bool fullscreen;
			bool clickToolbarShow;
			bool panel2Show;
			int lastWindow;
			bool isThumbnailBottom;
			int thumbnailPosition;
		};
	}
}