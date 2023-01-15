#pragma once
#include <ToolbarWindow.h>
using namespace Regards::Window;

class CRegardsBitmap;

namespace Regards::Scanner
{
	class CToolbarPDF : public CToolbarWindow
	{
	public:
		CToolbarPDF(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical);
		~CToolbarPDF() override;

	private:
		void EventManager(const int& id) override;
	};
}
