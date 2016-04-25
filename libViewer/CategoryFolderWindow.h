#pragma once
#include <PaneInterface.h>
#include <ScrollbarWnd.h>
#include <Pane.h>
#include <ClickToolbar.h>
#include <TreeWindow.h>
#include "CategoryWnd.h"
#include "ViewerParam.h"
#include <WindowMain.h>
#include <StatusBarInterface.h>
#include <thread>
#include <Photos.h>
using namespace std;
using namespace Regards::Window;
using namespace Regards::Viewer;
using namespace Regards::Control;

class CFindPhotoCriteria;
class CFolderCatalog;

namespace Regards
{
	namespace Viewer
	{
		class CCategoryFolderWindow : public CWindowMain
		{
		public:
			CCategoryFolderWindow(wxWindow* parent, wxWindowID id, IStatusBarInterface * statusBarViewer);
			~CCategoryFolderWindow();

            void UpdateScreenRatio();
            void InitSaveParameter();
			void Init(const int64_t &numFolder);
            wxString GetSqlRequest();
            void UpdateCriteria();
            void RefreshFilter();
            
		private:
			
			void RefreshCriteriaSearch(wxCommandEvent& event);
			void OnIdle(wxIdleEvent& evt);
			void Resize();
			static void FindPhotoCriteria(CFindPhotoCriteria * findPhotoCriteria);
			void RefreshThreadFolder(CFolderCatalog * folder);
			void CriteriaPhotoUpdate(wxCommandEvent& event);
           
            
			CScrollbarWnd * catalogWndScroll = nullptr;
			CTreeWindow * treeWindow = nullptr;
			CCategoryWnd * catalogWndOld = nullptr;
			CViewerParam * explorerconfig = nullptr;
			int oldPos = 0;
			bool update = false;
			PhotosVector photosVector;
			int numImage = 0;
			bool traitementEnd = true;
			PhotosVector::const_iterator it;
			int numProcess = 0;
			bool refreshFolder = false;
			IStatusBarInterface * statusBarViewer = nullptr;
			int numFolder = 0;
		};
	}
}

