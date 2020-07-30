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

			CPane(wxWindow* parent, wxWindowID id, CPaneInterface * paneInterface, const int &idPane, const CThemePane & theme, const bool& refreshButton);
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
			virtual void ClosePane();
			virtual void RefreshPane();
			void Resize();
			void ShowOtherWindow();

			void SetTitleBarVisibility(const bool &visible);
			int GetTitleHeight();

		protected:
			void OnMouseMove(wxMouseEvent& event);
			void RefreshData(wxCommandEvent& event);
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

