#pragma once
#include <ToolbarWindow.h>
#include <string>
using namespace std;
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CFiltreToolbar : public CToolbarWindow
		{
		public:
			CFiltreToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme);
			~CFiltreToolbar();
			void SetNumFiltre(const int &numFiltre);

		private:

			void EventManager(const int &id);
			int numFiltre = 0;

		};
	}
}
