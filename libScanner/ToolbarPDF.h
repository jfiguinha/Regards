#pragma once
#include <ToolbarWindow.h>
using namespace Regards::Window;

;

namespace Regards::Scanner
{
	class CToolbarPDF : public CToolbarWindow
	{
	public:
		CToolbarPDF(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical);
		~CToolbarPDF() = default;

	private:
		void EventManager(const int& id) override;
		std::unique_ptr<CToolbarButton> screen;
		std::unique_ptr<CToolbarButton> save;
		std::unique_ptr<CToolbarButton> addpage;
		std::unique_ptr<CToolbarButton> deletepage;
		std::unique_ptr<CToolbarButton> print;
		std::unique_ptr<CToolbarButton> extract;
		std::unique_ptr<CToolbarButton> imageFirst;
	};
}
