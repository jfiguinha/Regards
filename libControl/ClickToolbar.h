#pragma once
#include <Theme.h>
#include <ToolbarInterface.h>
#include <ToolbarWindow.h>
using namespace Regards::Window;
using namespace std;

#define WM_CLICK 86

namespace Regards
{
	namespace Control
	{
		class CClickToolbar : public CToolbarWindow
		{
		public:
			CClickToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * interfaceToolbar, const int &idMessage);
			virtual ~CClickToolbar();
			void SetVertical(const bool &vertical);

		private:

			void EventManager(const int &id);
			
			CToolbarInterface * interfaceToolbar;
			int idObject;
		};
	}
}


