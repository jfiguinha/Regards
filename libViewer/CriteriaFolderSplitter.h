#pragma once
#include <SplitterWithPanel.h>
#include <directoryctrl.h>
#include "CategoryFolderWindow.h"
#ifdef WIN32FOLDERWND
#include <TreeFolder.h>
#endif
#include "ViewerParam.h"
using namespace std;
using namespace Regards::Window;
using namespace Regards::Control;

namespace Regards
{
	namespace Viewer
	{

		class CCriteriaFolderSplitter :public CSplitterWithPanel
		{
		public:
			CCriteriaFolderSplitter(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarInterface, const CThemeSplitter & theme, const CThemeToolbar & themeInfosToolbar, const bool &horizontal = true);
			~CCriteriaFolderSplitter();
			wxString GetSqlRequest();
			void ScreenMode();
            void HidePanel();
            
		private:

			void ShowPanelCriteria(wxCommandEvent& event);
			void ShowPanelFolder(wxCommandEvent& event);
			void SetFolder(wxCommandEvent& folderEvent);
			void OnSelChanged(wxCommandEvent& aEvent);
			void SaveParameter(wxCommandEvent& event);
			void UpdateCriteria(wxCommandEvent& event);


			int posBarInfos;
			CCategoryFolderWindow * categoryFolderWnd;
			wxGenericDirCtrl * folderWnd;
			CViewerParam * viewerconfig;
			bool fullscreen;
			int lastWindow;
		};
	}
}