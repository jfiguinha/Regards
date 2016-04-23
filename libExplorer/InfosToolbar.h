#pragma once
#include <ToolbarWindow.h>
#include <ToolbarInterface.h>
#include <ToolbarTexte.h>
#include <string>
#include <vector>
using namespace std;
using namespace Regards::Window;

namespace Regards
{
	namespace Explorer
	{
		class CInfosToolbar : public CToolbarWindow
		{
		public:
			CInfosToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface);
			~CInfosToolbar();
			void SetInfosActif();
			void SetMapActif();
			void SetMapInactif();

            void SetInfosPush();
            void SetMapPush();
		private:

			void OnSize(wxSizeEvent& event);
			void EventManager(const int &id);
			CToolbarInterface * toolbarInterface;
			CToolbarTexte * infos = nullptr;
			CToolbarTexte * map = nullptr;
			int width = 0;
			int height = 0;
		};
	}
}

