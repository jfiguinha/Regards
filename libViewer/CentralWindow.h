#pragma once
#include <Splitter.h>
#include "PanelPhotoWnd.h"
#include "ViewerWindow.h"
#include "ListPicture.h"
#ifndef __NOFACE_DETECTION__
#include "ListFace.h"
#endif
#include "ViewerParam.h"
#include <PanelWithClickToolbar.h>
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;
class CImageLoadingFormat;
#define CATALOG_FILTER 2

namespace Regards
{
	namespace Viewer
	{
		class CCentralWindow :public CSplitter
		{
		public:
			CCentralWindow(wxWindow* parent, wxWindowID id, const CThemeSplitter & theme, CImageList * imageList, const bool &horizontal = true);
			~CCentralWindow();

			void UpdateScreenRatio();
			void FullscreenMode();
			void ScreenMode();

		private:

			void OnRefreshData(wxCommandEvent& event);
			void SetListeFile(wxCommandEvent& event);
			void ChangeTypeAffichage(wxCommandEvent& event);
			void OnResize(wxCommandEvent& event);
			void SetMode(wxCommandEvent& event);
			void RedrawBarPos();

			int posBarInfos;

			CPanelWithClickToolbar * panelSearch;
			CPanelPhotoWnd * panelPhotoWnd;
			
			CMainParam * viewerconfig;

			//Viewer window
			CViewerWindow * viewerWindow;
			//Explorer Window
			CListPicture * listPicture;
			//Face List
#ifndef __NOFACE_DETECTION__
			CListFace * listFace;
#endif

			PhotosVector photoVector;

			int windowMode;
			bool fullscreen;
		};
	}
}