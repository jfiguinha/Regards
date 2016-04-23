#pragma once
#include <ToolbarWindow.h>
#include <string>
using namespace std;
using namespace Regards::Window;

namespace Regards
{
	namespace Explorer
	{
		class CMainToolbar : public CToolbarWindow
		{
		public:
			CMainToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme);
			~CMainToolbar();

		private:

			void EventManager(const int &id);
		};
	}
}
