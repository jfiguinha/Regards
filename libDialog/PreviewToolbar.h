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
		class CPreviewToolbar : public CToolbarWindow,public  CSliderInterface
		{

		public:

			CPreviewToolbar(wxWindow* parent, wxWindowID id, wxWindowID viewerId, const CThemeToolbar & theme, const bool& vertical);
			~CPreviewToolbar();
			//void SetBitmapDisplayPt(CBitmapWndViewer * bitmapWindow);
			void SetTrackBarPosition(const int &iPos);
			void SetTabValue(vector<int> value);
			void ZoomOn();
			void ZoomOut();
			void SlidePosChange(const int &position, const wxString &key);
			void MoveSlider(const int64_t &position){};
			void ZoomPos(const int &position);
			void ClickButton(const int &id);
            void ChangeZoomInPos();
            void ChangeZoomOutPos();
            
		private:
		
			void EventManager(const int &id);
			CToolbarSlide * slide;
			wxWindowID parentId;
			
		};
	}
}

