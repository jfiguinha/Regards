#pragma once
#include <ToolbarWindow.h>
#include <ToolbarInterface.h>
#include <ToolbarTexte.h>
#include <ConfigParam.h>
using namespace Regards::Window;


namespace Regards
{
	namespace Scanner
	{

		class CToolbarInfos : public CToolbarWindow
		{
		public:
			CToolbarInfos(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface, const bool& vertical);
			~CToolbarInfos();
			void SetInfosActif();
            void SetInfosPush();
		private:

			virtual void Resize();
			void EventManager(const int &id);

			CToolbarInterface * toolbarInterface;
			CToolbarTexte * infos;

		};


	}
}