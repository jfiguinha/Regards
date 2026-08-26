#pragma once
#include <ToolbarWindow.h>
#include <ToolbarTexte.h>
using namespace Regards::Window;

namespace Regards::Viewer
{
	class CToolbarKeyword : public CToolbarWindow
	{
	public:
		CToolbarKeyword(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical);
		~CToolbarKeyword() = default;

		void ClickButton(const int& id)
		{
		};

	private:

		void SendEventMessage();
		void EventManager(const int& id) override;
		std::unique_ptr<CToolbarButton> add = nullptr;
		std::unique_ptr<CToolbarTexte> libelle = nullptr;
	};
}
