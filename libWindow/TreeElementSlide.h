#pragma once
#include "TreeElement.h"
#include "TreeElementSlideInterface.h"
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
			void SetTabValue(vector<CTreeElementValue *> * value);
			void SetExifKey(const wxString &exifKey);
			void SetVisible(const bool &visible);
			void MouseOver(wxDC * deviceContext, const int &x, const int &y);
			void ClickElement(wxWindow * window, const int &x, const int &y);
			void UnclickElement(wxWindow * window, const int &x, const int &y);
			void SetInitValue(CTreeElementValue * value);

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

			bool TestEgality(CTreeElementValue * value1, CTreeElementValue * value2, int type);
			bool FindCirclePos(wxWindow * window, const int &y, const int &x);
			void ClickLeftPage(const int &x);
			void ClickRightPage(const int &x);
			void CalculZoomPosition(const int &x);
			void CalculPositionButton(const int &x);
			void CalculPositionButton();
			void DrawShapeElement(wxDC * deviceContext, const wxRect &rc);
			void RenderSlide(wxDC * deviceContext, const int &width, const int &height, const int &x, const int &y);
			CTreeElementValue * GetFirstValue();
			CTreeElementValue * GetLastValue();
			CTreeElementValue * GetPositionValue();
			void TestMaxMinValue();

			wxCursor hCursorHand;
			wxImage button;
			wxImage buttonPlus;
			wxImage buttonMoins;
			wxRect positionButton;
			wxRect positionSlider;
			
			vector<CTreeElementValue *> * tabValue;
			wxString exifKey;
			int height;
			int position;
			CTreeElementSlideInterface * eventInterface;
			bool mouseBlock;
			bool captureBall;
			CThemeTreeSlide themeSlide;
			wxRect plusPos;
			wxRect moinsPos;
			wxRect slidePos;

		};
	}
}

