#pragma once
#include <ToolbarWindow.h>
#include <ToolbarSlide.h>
#include <SliderInterface.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Viewer
	{
		class CThumbnailFacePertinenceToolBar : public CToolbarWindow, public CSliderInterface
		{
		public:
			CThumbnailFacePertinenceToolBar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme);
			virtual ~CThumbnailFacePertinenceToolBar();

			void SetTrackBarPosition(const int &iPos);
			void SetTabValue(vector<int> value);
			void ZoomOn();
			void ZoomOff();
			void ZoomPos(const int &position);

			void SlidePosChange(const int &position, const wxString &key);
			void MoveSlider(const int64_t &position){};
			void ClickButton(const int &id){};

		private:

			void OnChangeValue();
			void EventManager(const int &id);

			CToolbarSlide * slide;

		};
	}
}
