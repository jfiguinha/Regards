#pragma once
#include <ToolbarWindow.h>
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
			int numFiltre;

		};
	}
}
