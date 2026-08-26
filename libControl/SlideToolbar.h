#pragma once
#include <ToolbarWindow.h>
#include <SliderInterface.h>
#include <ToolbarSlide.h>
using namespace Regards::Window;

#define SPEAKER_MODE 1
#define ZOOM_MODE 2

namespace Regards::Video
{
	class CSlideToolbar : public CToolbarWindow, public CSliderInterface
	{
	public:
		CSlideToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme);
		~CSlideToolbar() = default;
		void SetComposantId(const int& id);
		void SetTrackBarPosition(const int& iPos) override;
		void SetTabValue(vector<int> value);
		void SetSpeakerMode();
		void SetZoomMode();
		void ZoomOn();
		void ZoomOut();
		int GetMode();
		void SlidePosChange(const int& position, const wxString& key) override;

		void MoveSlider(const int64_t& position) override
		{
		};
		void ZoomPos(const int& position) override;
		void ChangeZoomOutPos(const int& position);
		void ChangeZoomInPos(const int& position);
		void ClickButton(const int& id) override;

	private:
		void EventManager(const int& id) override;
		std::unique_ptr<CToolbarButton> speaker;
		std::unique_ptr<CToolbarButton> zoom;
		std::unique_ptr<CToolbarSlide> slide;
		std::unique_ptr<CToolbarButton> shrink;
		std::unique_ptr<CToolbarButton> moins;
		std::unique_ptr<CToolbarButton> plus;
		std::unique_ptr<CToolbarButton> cross;
		int positionTrackBar;
		int id;
	};
}
