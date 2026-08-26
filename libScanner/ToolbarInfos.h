#pragma once
#include <ToolbarWindow.h>
#include <ToolbarInterface.h>
#include <ToolbarTexte.h>
using namespace Regards::Window;


namespace Regards::Scanner
{
	class CToolbarInfos : public CToolbarWindow
	{
	public:
		CToolbarInfos(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, CToolbarInterface* toolbarInterface,
		              const bool& vertical);
		~CToolbarInfos() = default;
		void SetInfosActif();
		void SetInfosPush();
		void SetOcrPush();
		void SetOcrActif();

		void SetEffectParameterInactif();
		void SetEffectParameterActif(const wxString& libelle);

		void SetEffectActif();
		void SetEffectInactif();

		void SetEffectPush();
		void SetEffectParameterPush();

		void SetHistoryPush();
		void SetEditorPush();

	private:
		void Resize() override;
		void EventManager(const int& id) override;

		CToolbarInterface* toolbarInterface;
		std::unique_ptr<CToolbarTexte> infos;
		std::unique_ptr<CToolbarTexte> ocrText;
		std::unique_ptr<CToolbarTexte> history;
		std::unique_ptr<CToolbarTexte> effect;
		std::unique_ptr<CToolbarTexte> editorParam;
		std::unique_ptr<CToolbarTexte> effectParameter;
	};
}
