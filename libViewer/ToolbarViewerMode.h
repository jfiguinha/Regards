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
		~CToolbarViewerMode() = default;
		void SetFaceWindowPush();
		void SetViewerWindowPush();
		void SetExplorerWindowPush();
		void SetPictureWindowPush();
        void SetUpdateVisible(const bool& isVisible);
        
	private:

		void EventManager(const int& id) override;
		void DisableAll();
		CToolbarInterface* toolbarInterface = nullptr;
		std::unique_ptr<CToolbarTexte> facemode = nullptr;
		std::unique_ptr<CToolbarTexte> viewermode = nullptr;
		std::unique_ptr<CToolbarTexte> explorermode = nullptr;
		std::unique_ptr<CToolbarTexte> picturemode = nullptr;
        std::unique_ptr<CToolbarTexte> imageNewVersion = nullptr;
	};
}
