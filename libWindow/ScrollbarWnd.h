#pragma once
#include <theme.h>
#include <WindowMain.h>
namespace Regards
{
	namespace Window
	{
		class CScrollInterface;
		class CScrollbarHorizontalWnd;
		class CScrollbarVerticalWnd;

		class CScrollbarWnd : public CWindowMain
		{
		public:
			CScrollbarWnd(wxWindow* parent, wxWindowID id, const wxString & windowName = "ScrollBar", const bool & autohidden = true);
			~CScrollbarWnd();

			void SetCentralWindow(CScrollInterface * scrollInterface, const CThemeScrollBar & theme);

			CScrollInterface * GetScrollPt();

			CScrollbarHorizontalWnd * GetHScrollbar();
			CScrollbarVerticalWnd * GetVScrollbar();
            
            void UpdateScreenRatio();

			void SetPageSize(const int &pageSize);
			void SetLineSize(const int &lineSize);
			void HideVerticalScroll();
			void HideHorizontalScroll();
			void ShowVerticalScroll();
			int GetBarWidth();
			int GetBarHeight();
			void SetControlSize(const int &width, const int &height, const bool &useScaleFactor = false);
			void SetPosition(const int &posX, const int &posY);
			void Resize();

			virtual int GetHeight();
			virtual int GetWidth();

			int GetPosLargeur();
			int GetPosHauteur();

		protected:

			void OnSize(wxSizeEvent& event);
			void OnEraseBackground(wxEraseEvent& event){};
            void OnMouseMove(wxMouseEvent& event);
            void OnHide(wxTimerEvent& event);
            void OnMouseLeave(wxMouseEvent& event);
            
			CScrollbarHorizontalWnd * scrollHorizontal;
			CScrollbarVerticalWnd * scrollVertical;
			CScrollInterface * scrollInterface;

			bool showV;
			bool showH;
            bool _showV = false;
            bool _showH = false;
			int width;
			int height;
            bool useScaleFactor = false;
			int posHauteur;
			int posLargeur;
			int controlHeight;
			int controlWidth;
            bool autohidden = false;
            wxTimer * loadingTimer;
		};
	}
}
