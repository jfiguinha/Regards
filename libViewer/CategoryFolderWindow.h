#pragma once
#include <theme.h>
#include <TreeWithScrollbar.h>
using namespace Regards::Window;

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
			CCategoryFolderWindow(wxWindow* parent, wxWindowID idCTreeWithScrollbarViewer,
			                      const CThemeScrollBar& themeScroll, const CThemeTree& theme);
			~CCategoryFolderWindow() override;

			void InitSaveParameter();
			void init();
			void UpdateCriteria(const bool& needToSendMessage);
			wxString GetWaitingMessage() override;
			void RefreshCriteriaSearch();
			wxString GetSqlRequest();

		private:
			void OnUpdateGpsInfos(wxCommandEvent& event);
			void CriteriaPhotoUpdate(wxCommandEvent& event);
			void RefreshCriteriaSearch(wxCommandEvent& event);
			void OnIdle(wxIdleEvent& evt);
			bool GetProcessEnd() override;

			static void FindPhotoCriteria(CFindPhotoCriteria* findPhotoCriteria);
			static void FindGPSPhotoCriteria(CFindPhotoCriteria* findPhotoCriteria);
			void RefreshThreadFolder(CFolderCatalog* folder);
			void OnTimerRefresh(wxTimerEvent& event);
			void ProcessIdle() override;

			CCategoryWnd* catalogWndOld;
			CMainParam* explorerconfig;
			int oldPos;
			bool update;
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
			wxTimer* refreshTimer;
		};
	}
}
