#pragma once
#include "TitleBarInterface.h"
#include "WindowMain.h"


namespace Regards::Window
{
	class CTitleBar : public CWindowMain
	{
	public:
		CTitleBar(wxWindow* parent, wxWindowID id, CTitleBarInterface* titleBarInterface);
		~CTitleBar() override;

		int SetTooltipText(const wxString& tooltip);
		void SetTitle(const wxString& title);
		void SetClosable(const bool& value);
		void SetRefresh(const bool& value);
		void SetCenter(const bool& value);
		void UpdateScreenRatio() override;
		void SetTheme(CThemeTitleBar* themeTitle);
		void Redraw();

	protected:
		void Resize() override;

		void on_paint(wxPaintEvent& event);
		void OnMouseMove(wxMouseEvent& event);
		void OnLButtonDown(wxMouseEvent& event);

		void CreateBitmapRefreshOff();
		void CreateBitmapRefreshOn();
		void CreateBitmapCrossOn();
		void CreateBitmapCrossOff();

		bool isRefresh;
		bool isClosable;
		bool isCenter;
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
		CTitleBarInterface* titleBarInterface;
		CThemeTitleBar themeTitle;
	};
}
