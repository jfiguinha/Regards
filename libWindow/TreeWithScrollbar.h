#pragma once
#include <WindowMain.h>
using namespace std;

namespace Regards
{
	namespace Window
	{

		class CTreeWindow;
		class CScrollbarWnd;

		class CTreeWithScrollbar : public CWindowMain
		{
		public:
			CTreeWithScrollbar(const wxString& windowName, wxWindow* parent, wxWindowID id,
			                   const CThemeScrollBar& themeScroll, const CThemeTree& theme);
			~CTreeWithScrollbar(void) override;
			void UpdateScreenRatio() override;

		protected:
			void Resize() override;

			CScrollbarWnd* scrollWindow;
			CTreeWindow* treeWindow;
		};

	}
}
