#pragma once
#include <Splitter.h>
#include <Pane.h>
#include "ClickToolbar.h"
#include <ToolbarInterface.h>

using namespace std;
using namespace Regards::Window;

#define ALL_WINDOW 0
#define PANEL1_FILTER 1
#define PANEL2_FILTER 2

namespace Regards
{
	namespace Control
	{
		class CSplitterWithPanel :public CSplitter, public CPaneInterface, public CToolbarInterface
		{
		public:
			CSplitterWithPanel(wxWindow* parent, wxWindowID id, const CThemeSplitter & theme, const CThemeToolbar & themeInfosToolbar, const bool &horizontal);
			~CSplitterWithPanel();
			void SetHorizontal(const bool &horizontal);
			void UpdateScreenRatio();
			void ClickShowButton(const int &id);
			void ClosePane(const int &id);
			void ShowWindow();
			int GetPosition(const int &panel);
			void FixWindow(const int &numWindow, const int &size);
		protected:

			//void ShowWindow(const bool & showInfos);
			void ShowWindow(const int &id, const bool &show);
			virtual int GetDefaultPositionForWindow2();
			virtual int GetDefaultPositionForWindow1();
			int posBarInfos;
			CPane * paneWindow1;
			CClickToolbar * clickWindow1Toolbar;
			bool showclickWindow1Toolbar;
			CPane * paneWindow2;
			CClickToolbar * clickWindow2Toolbar;
			bool showclickWindow2Toolbar;
			bool fullscreen;
			int lastWindow;
			int numWindow;
			int size;
		};
	}
}