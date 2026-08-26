#pragma once
#include <ToolbarWindow.h>
#include <SliderInterface.h>
#include <ToolbarSlide.h>
using namespace Regards::Window;

namespace Regards::Control
{
	class CPreviewToolbar : public CToolbarWindow, public CSliderInterface
	{
	public:
		CPreviewToolbar(wxWindow* parent, wxWindowID id, wxWindowID viewerId, const CThemeToolbar& theme,
		                const bool& vertical);
		~CPreviewToolbar() override;
		//void SetBitmapDisplayPt(CBitmapWndViewer * bitmapWindow);
		void SetTrackBarPosition(const int& iPos) override;
		void SetTabValue(vector<int> value);
		void ZoomOn();
		void ZoomOut();
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
		std::unique_ptr<CToolbarSlide> slide;
		std::unique_ptr<CToolbarButton> newPicture;
		std::unique_ptr<CToolbarButton> oldPicture;
		std::unique_ptr<CToolbarButton> shrink;
		std::unique_ptr<CToolbarButton> moins;
		std::unique_ptr<CToolbarButton> plus;
		std::unique_ptr<CToolbarButton> cross;
		std::unique_ptr<CToolbarButton> editor;
		std::unique_ptr<CToolbarButton> rotate90;
		wxWindowID parentId;
	};
}
