#pragma once
#include <ToolbarWindow.h>
#include <ToolbarInterface.h>
#include <ToolbarTexte.h>
using namespace Regards::Window;


namespace Regards::Viewer
{
	class CToolbarViewerMode : public CToolbarWindow
	{
	public:
		CToolbarViewerMode(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, CToolbarInterface* toolbarInterface,
			const bool& vertical);
		~CToolbarViewerMode() override;
		void SetFaceWindowPush();
		void SetViewerWindowPush();
		void SetExplorerWindowPush();
		void SetPictureWindowPush();

	private:

		void EventManager(const int& id) override;
		void DisableAll();
		CToolbarInterface* toolbarInterface = nullptr;
		CToolbarTexte* facemode = nullptr;
		CToolbarTexte* viewermode = nullptr;
		CToolbarTexte* explorermode = nullptr;
		CToolbarTexte* picturemode = nullptr;
	};
}
