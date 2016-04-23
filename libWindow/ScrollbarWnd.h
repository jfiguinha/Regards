#pragma once
#include <Theme.h>
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

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
			void HideVerticalScroll();
			void HideHorizontalScroll();
			int GetBarWidth();
			int GetBarHeight();
			void SetControlSize(const int &width, const int &height);
			void SetPosition(const int &posX, const int &posY);
			void Resize();

			int GetPosLargeur();
			int GetPosHauteur();

		protected:

			void OnSize(wxSizeEvent& event);
			void OnEraseBackground(wxEraseEvent& event){};

			CScrollbarHorizontalWnd * scrollHorizontal = nullptr;
			CScrollbarVerticalWnd * scrollVertical = nullptr;
			CScrollInterface * scrollInterface = nullptr;

			bool showV = false;
			bool showH = false;
			int width = 0;
			int height = 0;

			int posHauteur = 0;
			int posLargeur = 0;
			int controlHeight = 0;
			int controlWidth = 0;
		};
	}
}
