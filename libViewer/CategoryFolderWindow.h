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
#include <Photos.h>
using namespace Regards::Window;
using namespace Regards::Viewer;
//using namespace Regards::Control;

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
			void Init();
            wxString GetSqlRequest();
			void UpdateCriteria(const bool &needToSendMessage);
            void RefreshFilter();
			wxString GetWaitingMessage();
			void RefreshCriteriaSearch();

		private:
			
			void FacePhotoUpdate(wxCommandEvent& event);
			void CriteriaPhotoUpdate(wxCommandEvent& event);
			void RefreshCriteriaSearch(wxCommandEvent& event);
			void OnIdle(wxIdleEvent& evt);
			bool GetProcessEnd();

			void Resize();
			static void FindPhotoCriteria(CFindPhotoCriteria * findPhotoCriteria);
            static void FindGPSPhotoCriteria(CFindPhotoCriteria * findPhotoCriteria);
			void RefreshThreadFolder(CFolderCatalog * folder);
			void OnTimerRefresh(wxTimerEvent& event);
			void ProcessIdle();

			CScrollbarWnd * catalogWndScroll;
			CTreeWindow * treeWindow;
			CCategoryWnd * catalogWndOld;
			CViewerParam * explorerconfig ;
			int oldPos;
			bool update ;
			int numImageFace;
			bool traitementEnd;
			int numProcess;
			int nbProcesseur;
			bool refreshFolder;
			IStatusBarInterface * statusBarViewer;
			bool needToSendMessage;
			bool threadDataProcess;
			bool noCategoryMessage;
			bool categoryMessage;
            //int nbPhotos;

            wxString urlServer;
			bool gpsLocalisationFinish;
            int nbGpsFile;
            wxTimer * refreshTimer;
		};
	}
}

