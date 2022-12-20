#pragma once
#include <ToolbarWindow.h>
#include <ToolbarInterface.h>
#include <ToolbarTexte.h>
using namespace Regards::Window;


namespace Regards
{
	namespace Viewer
	{

		class CToolbarPhoto : public CToolbarWindow
		{
		public:
			CToolbarPhoto(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme, CToolbarInterface * toolbarInterface, const bool& vertical);
			~CToolbarPhoto();
            void SetFolderPush();
            void SetCriteriaPush();
            void SetFaceListPush();

		private:

			virtual void Resize();
			void EventManager(const int &id);

			CToolbarInterface * toolbarInterface;
			CToolbarTexte * folder;
            CToolbarTexte * facelist;
			CToolbarTexte * criteria;

		};


	}
}