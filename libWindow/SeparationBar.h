#pragma once
#include "IMoveWindow.h"
#include "WindowMain.h"




namespace Regards
{
	namespace Window
	{
		class CSeparationBar : public CWindowMain
		{
		public:
			CSeparationBar(IMoveWindow * moveWindow, wxWindow* parent, wxWindowID id, const CThemeSeparationBar & theme);
			~CSeparationBar();
			void SetFastRender(const bool &fast);
			void SetHorizontal(const bool &horizontal);
			void UpdateScreenRatio();

		private:

			void OnPaint(wxPaintEvent& event);
			void OnMouseMove(wxMouseEvent& event);
			void OnLButtonDown(wxMouseEvent& event);
			void OnLButtonUp(wxMouseEvent& event);
			void OnMouseCaptureLost(wxMouseEvent& event);
            void OnMouseLeave(wxMouseEvent& event);
            void OnMouseHover(wxMouseEvent& event);

             
			//Variable
			void Resize();

			bool bSplitterMoving;
			bool horizontal;
			int position;
			int oldX;
			int oldY;
			bool fastRender;
			IMoveWindow * moveWindow;
			CThemeSeparationBar theme;
		};
	}
}

