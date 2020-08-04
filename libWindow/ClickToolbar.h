#pragma once
#include <theme.h>
//#include "ToolbarInterface.h"
#include "ToolbarWindow.h"
using namespace Regards::Window;
using namespace std;

//

namespace Regards
{
	namespace Window
	{
		class CToolbarInterface;

		class CClickToolbar : public CToolbarWindow
		{
		public:
			CClickToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * interfaceToolbar, const int &idMessage, const bool &vertical);
			virtual ~CClickToolbar();
			void SetVertical(const bool &vertical);
			int GetWidth();
			int GetHeight();
		private:

			void EventManager(const int &id);
			CToolbarButton * screen;
			CToolbarInterface * interfaceToolbar;
			int idObject;
		};
	}
}


