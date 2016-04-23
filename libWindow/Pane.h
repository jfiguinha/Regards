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
			~CPane();

			int SetTooltipText(const wxString & tooltip);

			void SetTitle(const wxString & title);

			void SetOtherWindow(wxWindow * window)
			{
				hWndOther = window;
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


		private:

			CTitleBar * titleBar = nullptr;
			wxWindow * hWndOther = nullptr;
			CPaneInterface * paneInterface = nullptr;
			int idPane;
			bool titleBarVisible = true;
			CThemePane themePane;
		};
	}
}

