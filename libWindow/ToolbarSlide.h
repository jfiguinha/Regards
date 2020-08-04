#pragma once
#include <theme.h>
#include "ToolbarElement.h"
//#include "SliderInterface.h"

namespace Regards
{
	namespace Window
	{
		class CSliderInterface;

		class CToolbarSlide : public CToolbarElement
		{
		public:
			CToolbarSlide(const CThemeSlider & themeSlider, CSliderInterface * eventInterface);
			virtual ~CToolbarSlide();
			void Resize(const int &tailleX, const int &tailleY);
			void SetPosition(const int &iPos);
			void SetTabValue(vector<int> value);
			void DrawButton(wxDC * dc, const int &x, const int &y);
			int GetPosition();
			int GetNbValue();
			void ZoomPos(const int &position);
			int GetWidth();
			int GetHeight();
			void SetBackgroundBitmap(const wxBitmap & background);
			bool MouseOver(wxDC * context, const int &x, const int &y);
			void UnclickElement(wxWindow * window, const int &x, const int &y);
			void ClickElement(wxWindow * window, const int &x, const int &y);
			int GetPositionValue();

		private:

			void CalculZoomPosition(const int &x);
			void CalculPositionButton(const int &x);
			void CalculPositionButton();
			
			bool FindCirclePos(wxWindow * window, const int &y, const int &x);
			void ClickLeftPage(const int &x);
			void ClickRightPage(const int &x);
			void DrawShapeElement(wxDC * deviceContext, const wxRect &rc);
			void RenderSlide(wxDC * deviceContext, const int &width, const int &height, const int &x, const int &y);

			int GetFirstValue();
			int GetLastValue();

			
			wxBitmap background;
			CSliderInterface * eventInterface;
			CThemeSlider themeSlider;
			wxCursor hCursorHand;
			wxImage button;
			wxRect positionButton;
			wxRect positionSlider;
			bool mouseBlock;
			bool captureBall;
			bool colorBackground;
			int position;
			vector<int> tabValue;
			wxRect posRectangle;
		};

	}
}
