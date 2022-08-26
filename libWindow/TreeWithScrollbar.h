#pragma once
#include <WindowMain.h>
#include "TitleBarInterface.h"
using namespace std;

namespace Regards
{
	namespace Window
	{

		class CTreeWindow;
		class CScrollbarWnd;
		class CTitleBar;

		class CTreeWithScrollbar : public CWindowMain, CTitleBarInterface
		{
		public:
			CTreeWithScrollbar(const wxString& windowName, wxWindow* parent, wxWindowID id,
			                   const CThemeScrollBar& themeScroll, const CThemeTree& theme, const wxString& label = "", const bool &showTitle = false);
			~CTreeWithScrollbar(void) override;
			void UpdateScreenRatio() override;
			void ClosePane() {};
			void RefreshPane() {};
			void SetLabel(const wxString& label);

		protected:
			void Resize() override;
			bool showTitle = false;
			CScrollbarWnd* scrollWindow;
			CTreeWindow* treeWindow;
			CTitleBar* titleBar;
		};

	}
}
