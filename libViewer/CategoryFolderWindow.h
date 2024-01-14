#pragma once
#include <theme.h>
#include <TreeWithScrollbar.h>
using namespace Regards::Window;

class CFindPhotoCriteria;
class CFolderCatalog;
class CCategoryFolderWindowPimpl;

namespace Regards::Viewer
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
		void UpdateCriteria(const bool& need_to_send_message);
		wxString GetWaitingMessage() override;
		void RefreshCriteriaSearch();
		wxString GetSqlRequest();

	private:
		void OnUpdateGpsInfos(wxCommandEvent& event);
		void CriteriaPhotoUpdate(wxCommandEvent& event);
		void RefreshCriteriaSearch(wxCommandEvent& event);
		void OnRefreshFolder(wxCommandEvent& event);
		void OnIdle(wxIdleEvent& evt) override;
		bool GetProcessEnd() override;

		static void FindPhotoCriteria(CFindPhotoCriteria* findPhotoCriteria);
		static void FindGPSPhotoCriteria(CFindPhotoCriteria* findPhotoCriteria);
		void RefreshThreadFolder(CFolderCatalog* folder);
		void OnTimerRefresh(wxTimerEvent& event);
		void ProcessIdle() override;
		time_t start;
        CCategoryFolderWindowPimpl * pimpl;
    
	};
}
