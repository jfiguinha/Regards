#pragma once
#include "TitleBarInterface.h"
#include <wx/cursor.h>
//#include <wx/TipWin.h>
#include "WindowMain.h"



namespace Regards
{
	namespace Window
	{
		class CTitleBar : public CWindowMain
		{
		public:
			CTitleBar(wxWindow* parent, wxWindowID id, CTitleBarInterface * titleBarInterface);
			~CTitleBar();

			int SetTooltipText(const wxString & tooltip);
			void SetTitle(const wxString & title);
			void SetClosable(const bool &value);
			void SetRefresh(const bool& value);
            void UpdateScreenRatio();
			void SetTheme(CThemeTitleBar * themeTitle);
			void Redraw();

		protected:

			void Resize();

			void OnPaint(wxPaintEvent& event);
			void OnMouseMove(wxMouseEvent& event);
			void OnLButtonDown(wxMouseEvent& event);

			void CreateBitmapRefreshOff();
			void CreateBitmapRefreshOn();
			void CreateBitmapCrossOn();
			void CreateBitmapCrossOff();

			bool isRefresh;
			bool isClosable;
			wxBitmap m_croixOff;
			wxBitmap m_croixOn;
			wxBitmap m_refreshOn;
			wxBitmap m_refreshOff;
			wxImage m_refreshButton;
			wxRect rcFermer;
			wxRect rcRefresh;
			bool mouseCapture;
			wxString libelle;
			wxString tooltip;
			wxString refreshtip;
			CTitleBarInterface * titleBarInterface;
			CThemeTitleBar themeTitle;
		};
	}
}

