#ifndef __NOFACE_DETECTION__
#pragma once
#include <ToolbarWindow.h>
#include <ToolbarSlide.h>
#include <ToolbarTexte.h>
#include <SliderInterface.h>
using namespace Regards::Window;

namespace Regards::Viewer
{
	class CThumbnailFacePertinenceToolBar : public CToolbarWindow, public CSliderInterface
	{
	public:
		CThumbnailFacePertinenceToolBar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme,
		                                const bool& vertical);
		~CThumbnailFacePertinenceToolBar() = default;

		void SetTrackBarPosition(const int& iPos) override;
		void SetTabValue(vector<int> value);
		void ZoomOn();
		void ZoomOff();
		void ZoomPos(const int& position) override;

		void SlidePosChange(const int& position, const wxString& key) override;

		void MoveSlider(const int64_t& position) override
		{
		};

		void ClickButton(const int& id) override
		{
		};

	private:
		void OnChangeValue();
		void EventManager(const int& id) override;

		std::unique_ptr<CToolbarSlide> slide;
		std::unique_ptr<CToolbarTexte> toolbarText = nullptr;
		std::unique_ptr<CToolbarButton> moins = nullptr;
		std::unique_ptr<CToolbarButton> plus = nullptr;
	};
}
#endif
