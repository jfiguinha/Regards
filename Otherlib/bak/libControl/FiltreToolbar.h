#pragma once
#include <ToolbarWindow.h>
using namespace Regards::Window;

namespace Regards::Control
{
	class CFiltreToolbar : public CToolbarWindow
	{
	public:
		CFiltreToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical);
		~CFiltreToolbar() override;
		void SetNumFiltre(const int& numFiltre);

	private:
		void EventManager(const int& id) override;
		int numFiltre;
		//wxWindow * wxWindowPanel;
	};
}
