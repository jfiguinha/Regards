#pragma once
#include <ToolbarWindow.h>
using namespace Regards::Window;

namespace Regards::Scanner
{
	class CValidationToolbar : public CToolbarWindow
	{
	public:
		CValidationToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical);
		~CValidationToolbar() override;

	private:
		void EventManager(const int& id) override;
		wxWindow* parent;
	};
}
