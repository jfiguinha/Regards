#pragma once
#include <WindowMain.h>
#include "ScrollbarWnd.h"
#include "PaneInterface.h"
#include "ClickToolbar.h"
#include "ToolbarInterface.h"
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
			CPanelWithClickToolbar(wxWindow* parent, const wxString& windowName, wxWindowID id, const CThemePane& themePane,
				const CThemeToolbar& themeToolbar, const wxString& paneLibelle,
				const bool& isPanelVisible, const bool& refreshButton, const bool& vertical);
			~CPanelWithClickToolbar() override;
			wxWindow* GetPaneWindow();
			wxWindow* GetWindow();
			void UpdateScreenRatio() override;
			void SetWindow(CWindowMain* windowMain);
			bool IsPanelVisible();
			//CScrollbarWnd * GetScrollWindow();
			int GetWidth() override;
			int GetHeight() override;
			void SetTitle(const wxString& title);
			void ShowPanel();
			void HidePanel(const bool& refresh = true);
			void ClosePane(const int& id, const int& refresh = 1) override;
			void ClickShowButton(const int& id, const int& refresh = 1) override;
			static CPanelWithClickToolbar* CreatePanel(wxWindow* parent, const wxString& panelLabel,
				const wxString& windowName, const bool& isVisible,
				const int& idPanel, const bool& isVertical,
				const bool& refreshButton);

		protected:
			void ShowPane(wxCommandEvent& event);
			void ClosePaneEvent(wxCommandEvent& event);
			void RefreshData(wxCommandEvent& event);

			void RefreshPane(const int& id) override;
			void Resize() override;

			CPane* paneWindow;
			//CScrollbarWnd * scrollWindow;
			CWindowMain* mainWindow;
			CClickToolbar* clickWindow;
			bool isPanelVisible;
			bool isVertical;
		};
	}
}
