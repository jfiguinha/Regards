#pragma once
#include <theme.h>
#include <WindowMain.h>
namespace Regards
{
	namespace Window
	{
		class CScrollbarVerticalWnd : public CWindowMain
		{
		public:
			CScrollbarVerticalWnd(const wxString &windowName, wxWindow* parent, wxWindowID id, const CThemeScrollBar & theme);
			~CScrollbarVerticalWnd();

			int GetWidthSize();
			void ShowEmptyRectangle(const bool &show, const int &heightSize);

			bool DefineSize(const int &screenHeight, const int &pictureHeight);
			//bool SetPosition(const int &top);

			void SetPageSize(const int &pageSize);
			int GetPageSize();
			void SetLineSize(const int &lineSize);
			int GetLineSize();


			int GetPosition();

			int GetScreenHeight();
			int GetPictureHeight();

			bool UpdateScrollBar(const int &posHauteur, const int &screenHeight, const int &pictureHeight);

			bool IsMoving();
            
            void UpdateScreenRatio();

			void SetShowWindow(const bool &showValue);

			void ClickTopTriangle();
			void ClickBottomTriangle();
			void ClickTopPage();
			void ClickBottomPage();

			bool SetPosition(const int& top);

		protected:

			void PaintNow();
			void OnPaint(wxPaintEvent& event);
			void OnMouseMove(wxMouseEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void OnLButtonUp(wxMouseEvent& event);
			void OnMouseLeave(wxMouseEvent& event);
			void OnMouseHover(wxMouseEvent& event);
			void OnTimerTriangleTop(wxTimerEvent& event);
			void OnTimerTriangleBottom(wxTimerEvent& event);
			void OnTimerPageTop(wxTimerEvent& event);
			void OnTimerPageBottom(wxTimerEvent& event);
			void OnTimerStopMoving(wxTimerEvent& event);
			void OnMouseCaptureLost(wxMouseEvent& event);
			void OnEraseBackground(wxEraseEvent& event){};
			void SendTopPosition(const int &value);
			void DrawElement(wxDC * dc);

			void Resize();
			void CalculBarSize();



			bool FindTopTriangle(const int &yPosition, const int &xPosition);
			bool FindBottomTriangle(const int &yPosition, const int &xPosition);
			bool FindRectangleBar(const int &yPosition, const int &xPosition);

			void DrawTopTriangleElement(wxDC * dc, const wxRect &rc, const wxColour &colorTriangle);
			void DrawBottomTriangleElement(wxDC * dc, const wxRect &rc, const wxColour &colorTriangle);
			void DrawRectangleElement(wxDC * dc, const wxColour &colorBar);
			void MoveBar(const int &diffY, wxColour color);
			void SetIsMoving();
			bool TestMaxY();
			bool TestMinY();
			void FillRect(wxDC * dc, const wxRect &rc, const wxColour &color);

			int barSize;
			int barPosY;
			bool captureBar;
			int stepSize;
			bool showEmptyRectangle;
			int heightSize;

			int barStartY;
			int barEndY;
			int yPositionStart;
			int yPositionStartMove;
			bool moveScrollbar;

			wxRect rcPosTriangleTop;
			wxRect rcPosTriangleBottom;
			wxRect rcPosBar;

			int pictureHeight;
			int screenHeight;
			int pageSize;
			int lineSize;
			int pageSizeDefault;
			int lineSizeDefault;
			int currentYPos;

			bool m_bTracking;

			bool scrollMoving;

			wxTimer * triangleTop;
			wxTimer * triangleBottom;
			wxTimer * pageTop;
			wxTimer * pageBottom;
			wxTimer * stopMoving;

			CThemeScrollBar themeScroll;
            
            bool showTriangle = false;

			bool showWindow = true;
		};
	}
}

