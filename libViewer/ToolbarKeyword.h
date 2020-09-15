#pragma once
#include <ToolbarWindow.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CToolbarKeyword : public CToolbarWindow
		{
		public:
			CToolbarKeyword(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, const bool& vertical);
			virtual ~CToolbarKeyword();

			void ClickButton(const int &id){};

		private:

			void EventManager(const int &id);

		};
	}
}
