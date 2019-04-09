#pragma once
#include <theme.h>

namespace Regards
{
	namespace Window
	{
		class CScrollInterface;

		class CScrollbarHorizontalWnd : public wxWindow
		{
		public:
			CScrollbarHorizontalWnd(CScrollInterface * scrollInterface, wxWindow* parent, wxWindowID id, const CThemeScrollBar & theme);
			~CScrollbarHorizontalWnd();

			int GetHeightSize();

			void SetPageSize(const int &pageSize);
			int GetPageSize();
			void SetLineSize(const int &lineSize);
			int GetLineSize();

			bool DefineSize(const int &screenWidth, const int &pictureWidth);
			bool SetPosition(const int &left);

			int GetPosition();

			int GetScreenWidth();
			int GetPictureWidth();
			bool IsMoving();
			bool UpdateScrollBar(const int &posLargeur, const int &screenWidth, const int &pictureWidth);

            void UpdateScreenRatio();
            
		protected:

            void CallRefresh();
			void DrawElement(wxDC * dc);
            void OnRefresh(wxCommandEvent& event);
			void OnPaint(wxPaintEvent& event);
			void OnSize(wxSizeEvent& event);
			void OnMouseMove(wxMouseEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void OnLButtonUp(wxMouseEvent& event);
			void OnMouseLeave(wxMouseEvent& event);
			void OnMouseHover(wxMouseEvent& event);
			void OnTimerTriangleLeft(wxTimerEvent& event);
			void OnTimerTriangleRight(wxTimerEvent& event);
			void OnTimerPageLeft(wxTimerEvent& event);
			void OnTimerPageRight(wxTimerEvent& event);
			void OnTimerStopMoving(wxTimerEvent& event);
			void OnMouseCaptureLost(wxMouseEvent& event);
			void OnEraseBackground(wxEraseEvent& event){};

			void Resize();

			void ClickLeftTriangle();
			void ClickRightTriangle();
			void ClickLeftPage();
			void ClickRightPage();

			bool FindLeftTriangle(const int &yPosition, const int &xPosition);
			bool FindRightTriangle(const int &yPosition, const int &xPosition);
			bool FindRectangleBar(const int &yPosition, const int &xPosition);

			void DrawLeftTriangleElement(wxDC * dc, const wxRect &rc, const wxColour &colorTriangle);
			void DrawRightTriangleElement(wxDC * dc, const wxRect &rc, const wxColour &colorTriangle);
			void DrawRectangleElement(wxDC * dc, const wxColour &colorBar);
			void MoveBar(const int &diffX, wxColour color);
			void SetIsMoving();
			void CalculBarSize();

			void FillRect(wxDC * dc, const wxRect &rc, const wxColour &color);

			bool TestMaxX();
			bool TestMinX();

			int xPositionStartMove;

			wxRect rcPosTriangleLeft;
			wxRect rcPosTriangleRight;
			wxRect rcPosBar;

			int barSize;
			int barPosX;
			bool captureBar;
			int pictureWidth;
			int screenWidth;
			int pageSize;
			int lineSize;
			int xPositionStart;
			int barStartX;
			int barEndX;
			int pageSizeDefault;
			int lineSizeDefault;
			int currentXPos;

			bool m_bTracking;


			wxTimer * triangleLeft;
			wxTimer * triangleRight;
			wxTimer * pageLeft;
			wxTimer * pageRight;
			wxTimer * stopMoving;

			CThemeScrollBar themeScroll;

			bool scrollMoving;
			int width;
			int height;
			CScrollInterface * scrollInterface;
            
            bool showTriangle = false;
		};
	}
}


