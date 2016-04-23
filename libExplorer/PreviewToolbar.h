#pragma once
#include <ToolbarWindow.h>
#include <ToolbarInterface.h>
#include <string>
#include <vector>
using namespace std;
using namespace Regards::Window;

namespace Regards
{
	namespace Explorer
	{
		class CPreviewToolbar : public CToolbarWindow
		{
		public:
			CPreviewToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface);
			~CPreviewToolbar();

		private:

			void EventManager(const int &id);
			CToolbarInterface * toolbarInterface;
			CToolbarButton * imagePlayDiaporama;
		};
	}
}


