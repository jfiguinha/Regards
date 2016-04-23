#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <BitmapWnd.h>
#include "BitmapWndViewer.h"
#include <ToolbarWindow.h>
#include <SliderInterface.h>
#include <ToolbarSlide.h>
#include <string>
#include <vector>
using namespace std;
using namespace Regards::Window;

namespace Regards
{
	namespace Control
	{
		class CBitmapToolbar : public CToolbarWindow,public  CSliderInterface
		{

		public:

			CBitmapToolbar(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme);
			~CBitmapToolbar();
			void SetBitmapDisplayPt(CBitmapWndViewer * bitmapWindow);
			void SetTrackBarPosition(const int &iPos);
			void SetTabValue(vector<int> value);
			void ZoomOn();
			void ZoomOut();
			//void ShowCropButton();
			//void HideCropButton();
			void ShowSaveButton();
			void HideSaveButton();
			void SlidePosChange(const int &position, const wxString &key);
			void MoveSlider(const int64_t &position){};
			void ZoomPos(const int &position);
			void ClickButton(const int &id);

		private:
		
			void EventManager(const int &id);

			CToolbarButton * save;
			//CToolbarButton * crop;
			CToolbarButton * email;
			CToolbarSlide * slide;
			CBitmapWndViewer * bitmapWindow;
			
		};
	}
}

