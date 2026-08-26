#pragma once
#include <Splitter.h>
#include <Pane.h>
#include "ClickToolbar.h"
#include <ToolbarInterface.h>
#include <memory>

using namespace std;
using namespace Regards::Window;

namespace Regards::Control
{
	enum class PanelId : int
	{
		ALL_WINDOWS = 0,
		PANEL1 = 1,
		PANEL2 = 2
	};

	class CSplitterWithPanel : public CSplitter, public CPaneInterface, public CToolbarInterface
	{
	public:
		CSplitterWithPanel(wxWindow* parent, wxWindowID id, const CThemeSplitter& theme,
						   const CThemeToolbar& themeInfosToolbar, bool horizontal);
		~CSplitterWithPanel() override = default;

		void SetHorizontal(bool horizontal);
		void UpdateScreenRatio() override;
		void ClickShowButton(int id);
		void ClosePane(int id);
		void ShowWindow();
		int GetPosition(int panel) const;
		void FixWindow(int numWindow, int size);

	protected:
		void ShowWindow(int id, bool show);
		virtual int GetDefaultPositionForWindow2() const;
		virtual int GetDefaultPositionForWindow1() const;

	private:
		// Private methods for panel visibility
		void ShowPanel1(bool show);
		void ShowPanel2(bool show);

		// Member variables with C++17 in-class initialization
		int posBarInfos = 0;
		std::unique_ptr<CPane> paneWindow1;
		std::unique_ptr<CClickToolbar> clickWindow1Toolbar;
		bool showclickWindow1Toolbar = true;
		std::unique_ptr<CPane> paneWindow2;
		std::unique_ptr<CClickToolbar> clickWindow2Toolbar;
		bool showclickWindow2Toolbar = true;
		bool fullscreen = false;
		PanelId lastWindow = PanelId::ALL_WINDOWS;
		int numWindow = 0;
		int size = 0;
	};
}
