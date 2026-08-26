#pragma once
#include <theme.h>
//#include "ToolbarInterface.h"
#include "ToolbarWindow.h"
using namespace Regards::Window;
using namespace std;

//
namespace Regards::Window
{
	class CToolbarInterface;

	class CClickToolbar : public CToolbarWindow
	{
	public:
		CClickToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, CToolbarInterface* interfaceToolbar,
		              const int& idMessage, const bool& vertical);
		~CClickToolbar()  = default;
		void SetVertical(const bool& vertical);
		int GetWidth() override;
		int GetHeight() override;

	private:
		void EventManager(const int& id) override;
		std::unique_ptr<CToolbarButton> screen;
		CToolbarInterface* interfaceToolbar;
		int idObject;
	};
}
