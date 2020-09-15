#pragma once
#include <WindowMain.h>
using namespace Regards::Window;


namespace Regards
{
	namespace Viewer
	{
		class CKeywordManagementWindow;
		class CToolbarKeyword;

		class CKeywordWndToolbar : public CWindowMain
		{
		public:
			CKeywordWndToolbar(wxWindow* parent, wxWindowID idCTreeWithScrollbar);
			~CKeywordWndToolbar();
			
			void UpdateScreenRatio();
			void Init(const wxString & filename);
			void Resize();

		private:
			
			CToolbarKeyword * toolbarKeyword;
			CKeywordManagementWindow * keywordWnd;
			wxString filename;
		};
	}
}

