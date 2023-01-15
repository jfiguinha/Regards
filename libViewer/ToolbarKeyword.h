#pragma once
#include <ToolbarWindow.h>
using namespace Regards::Window;

namespace Regards::Viewer
{
	class CToolbarKeyword : public CToolbarWindow
	{
	public:
		CToolbarKeyword(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical);
		~CToolbarKeyword() override;

		void ClickButton(const int& id)
		{
		};

	private:
		void EventManager(const int& id) override;
	};
}
