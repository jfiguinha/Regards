#pragma once
#include <ToolbarWindow.h>
using namespace Regards::Window;

namespace Regards::Control
{
	class CFiltreToolbar : public CToolbarWindow
	{
	public:
		CFiltreToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical);
		~CFiltreToolbar() = default;
		void SetNumFiltre(const int& numFiltre);

	private:

		void EventManager(const int& id) override;
		int numFiltre;
		std::unique_ptr<CToolbarButton> ok;
		std::unique_ptr<CToolbarButton> cancel;
	};
}
