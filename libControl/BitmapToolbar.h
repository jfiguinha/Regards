#pragma once
#include <ToolbarWindow.h>
#include <SliderInterface.h>
#include <ToolbarSlide.h>
using namespace Regards::Window;

namespace Regards::Control
{
	class CBitmapToolbar : public CToolbarWindow, public CSliderInterface
	{
	public:
		CBitmapToolbar(wxWindow* parent, wxWindowID id, wxWindowID viewerId, const CThemeToolbar& theme,
		               const bool& vertical, const bool& exportPicture);
		~CBitmapToolbar() = default;
		//void SetBitmapDisplayPt(CBitmapWndViewer * bitmapWindow);
		void SetTrackBarPosition(const int& iPos) override;
		void SetTabValue(vector<int> value);
		void ZoomOn();
		void ZoomOut();
		//void ShowExportButton();
		//void HideExportButton();
		void SlidePosChange(const int& position, const wxString& key) override;

		void MoveSlider(const int64_t& position) override
		{
		};
		void ZoomPos(const int& position) override;
		void ClickButton(const int& id) override;
		void ChangeZoomInPos();
		void ChangeZoomOutPos();

	private:
		void EventManager(const int& id) override;

		std::unique_ptr<CToolbarButton> email;
		std::unique_ptr<CToolbarButton> printer;
		std::unique_ptr<CToolbarButton> shrink;
		std::unique_ptr<CToolbarButton> moins;
		std::unique_ptr<CToolbarButton> plus;
		std::unique_ptr<CToolbarSlide> slide;

		wxWindowID parentId;
		bool exportPicture;
	};
}
