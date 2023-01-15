#pragma once
#include <ToolbarWindow.h>
#include <ToolbarInterface.h>
#include <ToolbarTexte.h>
using namespace Regards::Window;


namespace Regards::Viewer
{
	class CToolbarPhoto : public CToolbarWindow
	{
	public:
		CToolbarPhoto(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, CToolbarInterface* toolbarInterface,
		              const bool& vertical);
		~CToolbarPhoto() override;
		void SetFolderPush();
		void SetCriteriaPush();
		void SetFaceListPush();

	private:
		void Resize() override;
		void EventManager(const int& id) override;

		CToolbarInterface* toolbarInterface;
		CToolbarTexte* folder;
		CToolbarTexte* facelist;
		CToolbarTexte* criteria;
	};
}
