#pragma once
#include <ToolbarWindow.h>
#include <ToolbarSlide.h>
#include <SliderInterface.h>
using namespace Regards::Window;

namespace Regards::Viewer
{
	class CListPicture;

	class CThumbnailToolBar : public CToolbarWindow, public CSliderInterface
	{
	public:
		CThumbnailToolBar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical);
		~CThumbnailToolBar() = default;

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
		void EventManager(const int& id) override;
		void PostEvent(wxEventType type);
		CListPicture * GetListPicture();
		void Zoom(int delta);
		void SendZoomPosition(int position);
	
		std::unique_ptr<CToolbarSlide> slide;
		std::unique_ptr<CToolbarButton> indexButton = nullptr;
		std::unique_ptr<CToolbarButton> exportButton = nullptr;
		std::unique_ptr<CToolbarButton> dateButton = nullptr;
		std::unique_ptr<CToolbarButton> geoButton = nullptr;
		std::unique_ptr<CToolbarButton> deleteButton = nullptr;
		std::unique_ptr<CToolbarButton> copy = nullptr;
		std::unique_ptr<CToolbarButton> moins = nullptr;
		std::unique_ptr<CToolbarButton> plus = nullptr;
	};
}
