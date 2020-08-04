#pragma once
//#include "ScrollbarWnd.h"
//#include "TreeWindow.h"
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

			CTreeWithScrollbar(const wxString &windowName, wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & theme);
			~CTreeWithScrollbar(void);
			void UpdateScreenRatio();

		protected:

			void Resize();

			CScrollbarWnd * scrollWindow;
			CTreeWindow * treeWindow;
		};

	}
}