#pragma once
#include <SplitterWithPanel.h>
#include "PreviewFaceSplitter.h"
#ifdef NORMAL
#include "ThumbnailFolder.h"
#else
#include "ListPicture.h"
#include "ListFace.h"
#endif
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

		class CPreviewThumbnailSplitter :public CSplitterWithPanel
		{
		public:
			CPreviewThumbnailSplitter(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, const CThemeToolbar & themeInfosToolbar, CImageList * imageList, const bool &horizontal = true);
			~CPreviewThumbnailSplitter();

			void SetEffect(const bool &effect);
			bool IsPanelInfosVisible();
			void SetVideo(const wxString &path);

			bool SetBitmap(CImageLoadingFormat * bitmap, const bool &isThumbnail, const bool &isAnimation);
			void FullscreenMode();
			void ScreenMode();
			void ShowToolbar();
			void ShowListFace();
			void ShowThumbnail();
			void HideToolbar();
			void SetDiaporamaMode();
			void SetNormalMode();
			void SetActifItem(const int &numItem, const bool &move);
			void StopLoadingPicture();
			void StartLoadingPicture(const int &numElement);
			void ShowFilter();
			wxString GetSqlRequest();
			bool IsPanelThumbnailVisible();
            void UpdateScreenRatio();
            void HidePanel();



		private:

			void SetThumbnailBottom(wxCommandEvent& aEvent);
			void SetThumbnailRight(wxCommandEvent& aEvent);
			void ShowPanelThumbnail(wxCommandEvent& aEvent);
			void HidePanelThumbnail(wxCommandEvent& aEvent);
            void CloseThumbnailPane();
			int posBarInfos;
			CPreviewFaceSplitter * previewFaceSplitter;
			CListPicture * listPicture;
			CViewerParam * viewerconfig;
			bool fullscreen;
			bool clickToolbarShow;
			bool panel2Show;
			int lastWindow;
            bool isThumbnailVisible;
			bool isThumbnailBottom;
			int thumbnailPosition;
		};
	}
}