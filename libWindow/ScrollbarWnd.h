#pragma once
#include <theme.h>

namespace Regards
{
	namespace Window
	{
		class CScrollInterface;
		class CScrollbarHorizontalWnd;
		class CScrollbarVerticalWnd;

		class CScrollbarWnd : public wxWindow
		{
		public:
			CScrollbarWnd(wxWindow* parent, wxWindowID id);
			~CScrollbarWnd();

			void SetCentralWindow(CScrollInterface * scrollInterface, const CThemeScrollBar & theme);

			CScrollInterface * GetScrollPt();

			CScrollbarHorizontalWnd * GetHScrollbar();
			CScrollbarVerticalWnd * GetVScrollbar();
            
            void UpdateScreenRatio();

			void SetPageSize(const int &pageSize);
			void SetLineSize(const int &lineSize);
			int GetBarWidth();
			int GetBarHeight();
			void SetControlSize(const int &width, const int &height, const bool &useScaleFactor = false);
			void SetPosition(const int &posX, const int &posY);
			void Resize();

			int GetPosLargeur();
			int GetPosHauteur();

		protected:

			void OnHide(wxTimerEvent& event);
			void OnSize(wxSizeEvent& event);
			void OnEraseBackground(wxEraseEvent& event){};
			void OnMouseMove(wxCommandEvent& event);

			CScrollbarHorizontalWnd * scrollHorizontal;
			CScrollbarVerticalWnd * scrollVertical;
			CScrollInterface * scrollInterface;

			wxTimer* loadingTimer;
			bool showV;
			bool showH;
			int width;
			int height;
            bool useScaleFactor = false;
			int posHauteur;
			int posLargeur;
			int controlHeight;
			int controlWidth;
		};
	}
}
