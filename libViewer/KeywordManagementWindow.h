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
		class CKeywordWnd;
		class CMainParam;

		class CKeywordManagementWindow : public CTreeWithScrollbar
		{
		public:
			CKeywordManagementWindow(wxWindow* parent, wxWindowID idCTreeWithScrollbarViewer, const CThemeScrollBar & themeScroll, const CThemeTree & theme);
			~CKeywordManagementWindow();

			void Init(const wxString & filename);

		private:
			void UpdateKeyword(wxCommandEvent& event);
			CKeywordWnd * keywordWndOld;
			wxString filename;
		};
	}
}

