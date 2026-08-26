#pragma once
#include <ToolbarWindow.h>
#include <ToolbarInterface.h>
#include <ToolbarTexte.h>
using namespace Regards::Window;


namespace Regards::Viewer
{
	class CToolbarInfos : public CToolbarWindow
	{
	public:
		CToolbarInfos(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, CToolbarInterface* toolbarInterface,
		              const bool& vertical);
		~CToolbarInfos() = default;
		void SetVideoToolbar();
		void SetPictureToolbar();
		void SetPictureThumbnailToolbar();

		void SetInfosActif();
		void SetMapActif();
		void SetMapInactif();
		void SetEffectParameterInactif();
		void SetEffectParameterActif(const wxString& libelle);

		void SetEffectActif();
		void SetEffectInactif();
		void SetCriteriaPush();

		void SetHistogramPush();
		void SetInfosPush();
		void SetMapPush();
		void SetEffectPush();
		void SetHistoryPush();
		void SetEffectParameterPush();
		void SetAudioVideoPush();
		void SetVideoEffectPush();

	private:
		void Resize() override;
		void EventManager(const int& id) override;

		CToolbarInterface* toolbarInterface = nullptr;

		std::unique_ptr<CToolbarTexte> infos = nullptr;
		std::unique_ptr<CToolbarTexte> history = nullptr;
		std::unique_ptr<CToolbarTexte> effect = nullptr;
		std::unique_ptr<CToolbarTexte> audiovideo = nullptr;
		std::unique_ptr<CToolbarTexte> videoeffect = nullptr;
		std::unique_ptr<CToolbarTexte> map = nullptr;
		std::unique_ptr<CToolbarTexte> effectParameter = nullptr;
		std::unique_ptr<CToolbarTexte> criteria = nullptr;
		std::unique_ptr<CToolbarTexte> histogramParameter = nullptr;
	};
}
