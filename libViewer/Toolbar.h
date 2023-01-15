#pragma once
#include <ToolbarWindow.h>
using namespace Regards::Window;

class CRegardsBitmap;

namespace Regards::Viewer
{
	class CToolbar : public CToolbarWindow
	{
	public:
		CToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical);
		~CToolbar() override;

	private:
		void EventManager(const int& id) override;
		bool NewVersionAvailable();
	};
}
