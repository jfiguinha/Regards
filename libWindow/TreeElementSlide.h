#pragma once
#include "TreeElement.h"
#include "TreeElementSlideInterface.h"
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
using namespace Regards::Window;


namespace Regards
{
	namespace Window
	{
		class CTreeElementSlide : public CTreeElement
		{
		public:
			CTreeElementSlide(CTreeElementSlideInterface * eventInterface);
			~CTreeElementSlide();

			CTreeElementSlide& operator=(const CTreeElementSlide &other);

			void DrawElement(wxDC * deviceContext, const int &x, const int &y);
			void SetTabValue(vector<int> value);
			void SetExifKey(const wxString &exifKey);
			void SetVisible(const bool &visible);
			void MouseOver(wxDC * deviceContext, const int &x, const int &y);
			void ClickElement(wxWindow * window, const int &x, const int &y);
			void UnclickElement(wxWindow * window, const int &x, const int &y);


			bool MouseBlock();
			void SetElementPos(const int &x, const int &y);
			void SetPosition(const int &position);
			void SetTheme(CThemeTreeSlide * theme);

			void SetZoneSize(const int &width, const int &height);

			void SetBackgroundColor(const wxColour &color);

			int GetElementWidth()
			{
				return themeSlide.GetElementWidth();
			}

			int GetElementHeight()
			{
				return height;
			}

			int GetWidth()
			{
				return themeSlide.GetWidth();
			}

			int GetHeight()
			{
				return height;
			}

		private:

            wxImage CreateFromSVG(const int & buttonWidth, const int & buttonHeight, const wxString &vectorCode);
			bool FindCirclePos(wxWindow * window, const int &y, const int &x);
			void ClickLeftPage(const int &x);
			void ClickRightPage(const int &x);
			void CalculZoomPosition(const int &x);
			void CalculPositionButton(const int &x);
			void CalculPositionButton();
			void DrawShapeElement(wxDC * deviceContext, const wxRect &rc);
			void RenderSlide(wxDC * deviceContext, const int &width, const int &height, const int &x, const int &y);
			int GetFirstValue();
			int GetLastValue();
			int GetPositionValue();
			
			wxCursor hCursorHand;
			wxImage button;
			wxRect positionButton;
			wxRect positionSlider;
			int position = 0;
			vector<int> tabValue;
			wxString exifKey = L"";
			CTreeElementSlideInterface * eventInterface = nullptr;
			bool mouseBlock;
			bool captureBall;
			CThemeTreeSlide themeSlide;
            int height = 0;
            bool isVector = true;
            wxString buttonVector;
		};
	}
}

