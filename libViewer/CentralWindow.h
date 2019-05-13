#pragma once
#include <Splitter.h>
#include "PanelPhotoWnd.h"
#include "ViewerWindow.h"
#include "ListPicture.h"
#include "ListFace.h"
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
			CCentralWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, CImageList * imageList, const bool &horizontal = true);
			~CCentralWindow();

			void UpdateScreenRatio();
			void FullscreenMode();
			void ScreenMode();
			wxString GetSqlRequest();
			void SetMode(const int &mode);


			void ShowFile();

		private:

			void SetListeFile(wxCommandEvent& event);
			void ChangeTypeAffichage(wxCommandEvent& event);
			void OnResize(wxCommandEvent& event);
			void RedrawBarPos();

			int posBarInfos;

			CPanelWithClickToolbar * panelSearch;
			CPanelPhotoWnd * panelPhotoWnd;
			
			CViewerParam * viewerconfig;

			//Viewer window
			CViewerWindow * viewerWindow;
			//Explorer Window
			CListPicture * listPicture;
			//Face List
			CListFace * listFace;


			PhotosVector photoVector;

			int windowMode;
			bool fullscreen;
		};
	}
}