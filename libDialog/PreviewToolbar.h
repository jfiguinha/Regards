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
			CToolbarSlide* slide;
			wxWindowID parentId;
		};
	}
}
