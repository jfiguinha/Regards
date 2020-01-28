#pragma once
#include <WindowMain.h>
#include "ScrollbarWnd.h"
#include "PaneInterface.h"
#include "ClickToolbar.h"
#include "ToolbarInterface.h"
#include <WindowMain.h>
#include "Pane.h"

using namespace Regards::Window;
#define PANE_WITHCLICKTOOLBAR 1

namespace Regards
{
	namespace Window
	{

		class CPanelWithClickToolbar : public CWindowMain, public CPaneInterface, public CToolbarInterface
		{
		public:
			CPanelWithClickToolbar(wxWindow* parent, const wxString& windowName, wxWindowID id, const CThemePane& themePane, const CThemeToolbar& themeToolbar, const wxString& paneLibelle, const bool& isPanelVisible, const bool &refreshButton, const bool &vertical);
			virtual ~CPanelWithClickToolbar();
			wxWindow * GetPaneWindow();
			void UpdateScreenRatio();
			void SetWindow(CWindowMain * windowMain);
			bool IsPanelVisible();
			//CScrollbarWnd * GetScrollWindow();
			int GetWidth();
			int GetHeight();
			void ShowPanel();
			void HidePanel(const bool& refresh = true);
			void ClosePane(const int &id);
			void ClickShowButton(const int &id);
			static CPanelWithClickToolbar * CreatePanel(wxWindow * parent, const wxString &panelLabel, const wxString &windowName, const bool &isVisible, const int &idPanel, const bool &isVertical, const bool &refreshButton);

		protected:

			void ShowPane(wxCommandEvent& event);
			void ClosePane(wxCommandEvent& event);
			
			
			void RefreshPane(const int& id);
			void Resize();

			CPane * paneWindow;
			//CScrollbarWnd * scrollWindow;
			CWindowMain* mainWindow;
			CClickToolbar * clickWindow;
			bool isPanelVisible;
		};


	}
}
