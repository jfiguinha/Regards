#pragma once
#include <theme.h>
#include "ToolbarElement.h"

class CMemBitmap;

namespace Regards::Window
{
	class CSliderInterface;

	class CToolbarSlide : public CToolbarElement
	{
	public:
		CToolbarSlide(const CThemeSlider& themeSlider, CSliderInterface* eventInterface);
		~CToolbarSlide() override;
		void Resize(const int& tailleX, const int& tailleY) override;
		void SetPosition(const int& iPos);
		void SetTabValue(vector<int> value);
		void DrawButton(wxDC* dc, const int& x, const int& y) override;
		int GetPosition();
		int GetNbValue();
		void ZoomPos(const int& position);
		int GetWidth() override;
		int GetHeight() override;
		void SetBackgroundBitmap(const wxBitmap& background);
		bool MouseOver(wxDC* context, const int& x, const int& y) override;
		void UnclickElement(wxWindow* window, const int& x, const int& y) override;
		void ClickElement(wxWindow* window, const int& x, const int& y) override;
		int GetPositionValue();

	private:
		void CalculZoomPosition(const int& x);
		void CalculPositionButton(const int& x);
		void CalculPositionButton();

		bool FindCirclePos(wxWindow* window, const int& y, const int& x);
		void ClickLeftPage(const int& x);
		void ClickRightPage(const int& x);
		void DrawShapeElement(wxDC* deviceContext, const wxRect& rc);
		void RenderSlide(wxDC* deviceContext, const int& width, const int& height, const int& x, const int& y);

		int GetFirstValue();
		int GetLastValue();

		CMemBitmap* pimpl;
		wxBitmap background;
		CSliderInterface* eventInterface;
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
