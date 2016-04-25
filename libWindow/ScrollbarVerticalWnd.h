#pragma once
#include <Theme.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

namespace Regards
{
	namespace Window
	{
		class CScrollInterface;

		class CScrollbarVerticalWnd : public wxWindow
		{
		public:
			CScrollbarVerticalWnd(CScrollInterface * scrollInterface, wxWindow* parent, wxWindowID id, const CThemeScrollBar & theme);
			~CScrollbarVerticalWnd();

			int GetWidthSize();
			void ShowEmptyRectangle(const bool &show, const int &heightSize);

			bool DefineSize(const int &screenHeight, const int &pictureHeight);
			bool SetPosition(const int &top);

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

		protected:

			void OnPaint(wxPaintEvent& event);
			void OnSize(wxSizeEvent& event);
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

			void DrawElement();

			void Resize();
			void CalculBarSize();

			void ClickTopTriangle();
			void ClickBottomTriangle();
			void ClickTopPage();
			void ClickBottomPage();

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

			bool scrollMoving = false;

			int width = 0;
			int height = 0;

			wxTimer * triangleTop = nullptr;
			wxTimer * triangleBottom = nullptr;
			wxTimer * pageTop = nullptr;
			wxTimer * pageBottom = nullptr;
			wxTimer * stopMoving = nullptr;

			CThemeScrollBar themeScroll;
			CScrollInterface * scrollInterface;
		};
	}
}

