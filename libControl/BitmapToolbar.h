#pragma once
#include <BitmapWnd.h>
#include <ToolbarWindow.h>
#include <SliderInterface.h>
#include <ToolbarSlide.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CBitmapToolbar : public CToolbarWindow, public CSliderInterface
		{
		public:
			CBitmapToolbar(wxWindow* parent, wxWindowID id, wxWindowID viewerId, const CThemeToolbar& theme,
			               const bool& vertical, const bool& exportPicture);
			~CBitmapToolbar() override;
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

			//CToolbarButton * save;
			CToolbarButton* export_button;
			//CToolbarButton * crop;
			CToolbarButton* email;
			CToolbarSlide* slide;
			wxWindowID parentId;
			bool exportPicture;
		};
	}
}
