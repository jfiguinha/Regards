#pragma once
/*
#include <PaneInterface.h>
#include <ScrollbarWnd.h>
#include <Pane.h>
#include <ClickToolbar.h>
#include <TreeWindow.h>
#include "CategoryWnd.h"
#include "ViewerParam.h"
#include <TreeWithScrollbar.h>
#include <Photos.h>
*/

#include <theme.h>
#include <TreeWithScrollbar.h>
using namespace Regards::Window;
//using namespace Regards::Viewer;
//using namespace Regards::Control;

class CFindPhotoCriteria;
class CFolderCatalog;

namespace Regards
{
	namespace Viewer
	{
		class CCategoryWnd;
		class CMainParam;

		class CCategoryFolderWindow : public CTreeWithScrollbar
		{
		public:
			CCategoryFolderWindow(wxWindow* parent, wxWindowID idCTreeWithScrollbarViewer, const CThemeScrollBar & themeScroll, const CThemeTree & theme);
			~CCategoryFolderWindow();

            void InitSaveParameter();
			void Init();
			void UpdateCriteria(const bool &needToSendMessage);
			wxString GetWaitingMessage();
			void RefreshCriteriaSearch();
			wxString GetSqlRequest();

		private:
			void OnUpdateGpsInfos(wxCommandEvent& event);
			void CriteriaPhotoUpdate(wxCommandEvent& event);
			void RefreshCriteriaSearch(wxCommandEvent& event);
			void OnIdle(wxIdleEvent& evt);
			bool GetProcessEnd();

			static void FindPhotoCriteria(CFindPhotoCriteria * findPhotoCriteria);
            static void FindGPSPhotoCriteria(CFindPhotoCriteria * findPhotoCriteria);
			void RefreshThreadFolder(CFolderCatalog * folder);
			void OnTimerRefresh(wxTimerEvent& event);
			void ProcessIdle();

			CCategoryWnd * catalogWndOld;
			CMainParam * explorerconfig ;
			int oldPos;
			bool update ;
			//int numImageFace;
			bool traitementEnd;
			int numProcess;
			int nbProcesseur;
			bool refreshFolder;

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

