#ifndef __NOFACE_DETECTION__
#pragma once
#include <ToolbarWindow.h>
#include <ToolbarSlide.h>
#include <SliderInterface.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CThumbnailFaceToolBar : public CToolbarWindow, public CSliderInterface
		{
		public:
			CThumbnailFaceToolBar(wxWindow* parent, wxWindowID id, const CThemeToolbar& theme, const bool& vertical);
			~CThumbnailFaceToolBar() override;

			void SetTrackBarPosition(const int& iPos) override;
			void SetTabValue(vector<int> value);
			bool ZoomOn();
			bool ZoomOff();
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

			CToolbarSlide* slide;
		};
	}
}
#endif
