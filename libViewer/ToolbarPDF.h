#pragma once
#include <ToolbarWindow.h>
using namespace Regards::Window;

class CRegardsBitmap;

namespace Regards
{
	namespace Viewer
	{
		class CToolbarPDF : public CToolbarWindow
		{
		public:
			CToolbarPDF(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, const bool& vertical);
			~CToolbarPDF();

		private:

			void EventManager(const int &id);

		};
	}
}
