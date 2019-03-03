#pragma once
#include "PaneInterface.h"
#include "TitleBarInterface.h"
#include "TitleBar.h"
#include "WindowMain.h"


namespace Regards
{
	namespace Window
	{
		class CPane : public CTitleBarInterface, public CWindowMain
		{
		public:

			CPane(wxWindow* parent, wxWindowID id, CPaneInterface * paneInterface, const int &idPane, const CThemePane & theme);
			virtual ~CPane();

			int SetTooltipText(const wxString & tooltip);

			void SetTitle(const wxString & title);

			void SetOtherWindow(CWindowMain * window)
			{
				hMainWndOther = window;
                hWndOther = window;
			};

			void SetOtherWindow(wxWindow * window)
			{
				hWndOther = window;
                hMainWndOther = nullptr;
			};
            
            void UpdateScreenRatio();

			void SetClosable(const bool &value);
			void ClosePane();
			void Resize();

			void SetTitleBarVisibility(const bool &visible);
			int GetTitleHeight();

		protected:

			void OnPaint(wxPaintEvent& event);
			void OnMouseMove(wxMouseEvent& event);

			void OnIdle(wxIdleEvent& evt);
		private:

			CTitleBar * titleBar;
            wxWindow * hWndOther;
			CWindowMain * hMainWndOther;
			CPaneInterface * paneInterface;
			int idPane;
			bool titleBarVisible;
			CThemePane themePane;
		};
	}
}

