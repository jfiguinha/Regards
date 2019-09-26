#pragma once
#include <ToolbarWindow.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CValidationToolbar : public CToolbarWindow
		{
		public:
			CValidationToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, const bool& vertical);
			~CValidationToolbar();

		private:

			void EventManager(const int &id);
			wxWindow * parent;
		};
	}
}
