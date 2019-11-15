#pragma once
#include <ToolbarWindow.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Scanner
	{
		class CFiltreToolbar : public CToolbarWindow
		{
		public:
			CFiltreToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, const bool& vertical);
			~CFiltreToolbar();
			void SetNumFiltre(const int &numFiltre);

		private:

			void EventManager(const int &id);
			int numFiltre;

		};
	}
}
