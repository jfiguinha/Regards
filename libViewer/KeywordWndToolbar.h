#pragma once
#include <WindowMain.h>
using namespace Regards::Window;


namespace Regards::Viewer
{
	class CKeywordManagementWindow;
	class CToolbarKeyword;

	class CKeywordWndToolbar : public CWindowMain
	{
	public:
		CKeywordWndToolbar(wxWindow* parent, wxWindowID idCTreeWithScrollbar);
		~CKeywordWndToolbar() override;

		void UpdateScreenRatio() override;
		void Init(const wxString& filename);
		void Resize() override;

	private:
		CToolbarKeyword* toolbarKeyword;
		CKeywordManagementWindow* keywordWnd;
		wxString filename;
	};
}
