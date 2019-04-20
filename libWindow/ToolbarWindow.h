#pragma once
#include "ToolbarButton.h"
#include "WindowMain.h"

namespace Regards
{
	namespace Window
	{
		#define NAVIGATOR_LEFT 1
		#define NAVIGATOR_CENTER 2
		#define NAVIGATOR_RIGHT 3

		class CToolbarWindow : public CWindowMain
		{
		public:

			CToolbarWindow(wxWindow* parent, wxWindowID id, const CThemeToolbar & theme);
			virtual ~CToolbarWindow();
			int GetHeight();
			int GetWidth();
			int GetNavigatorHeight();
			int GetNavigatorWidth();
			bool IsMouseOver();
            void UpdateScreenRatio();

            
		protected:

			virtual void EventManager(const int &id) = 0;
			virtual void Resize();

			void OnPaint(wxPaintEvent& event);
			void OnMouseMove(wxMouseEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void OnLButtonUp(wxMouseEvent& event);
			void OnMouseLeave(wxMouseEvent& event);
			void OnMouseHover(wxMouseEvent& event);
			void OnMouseCaptureLost(wxMouseEvent& event);
			void OnTimerPushButton(wxTimerEvent& event);
			void OnEraseBackground(wxEraseEvent& event){};

			void DrawBackground(wxDC * deviceContext);
			void DrawBackground(wxDC * deviceContext, const wxRect &rc);
			void GenerateNavigatorButton(wxDC * deviceContext);
			void RedrawElement(wxDC * dc, CToolbarElement * nav);

			wxBitmap hbmNavigator;

			VectorNavigatorElement navElement;
			CToolbarElement * navPush;
			CThemeToolbar themeToolbar;
            bool saveLastPush;
            
		private:

            void DrawButton(wxDC * dc, CToolbarElement * nav);
			void EmptyNavigator();
			int numButtonActif;
			bool m_bIconeOn;
			bool m_bMouseMove;
			bool m_bMouseOver;
            
            wxBitmap background;
            wxImage backPicture;
			wxTimer * pushButton;
		};
	}
}


