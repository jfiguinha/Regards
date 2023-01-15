#pragma once
#include <WindowMain.h>
using namespace Regards::Window;

namespace Regards::Introduction
{
	class CTitleIntro : public CWindowMain
	{
	public:
		CTitleIntro(wxWindow* parent, wxWindowID id, const CThemeIntroLogo& theme);
		~CTitleIntro() override;

		void SetTitle(const wxString& title);
		int GetHeight() override;
		void UpdateScreenRatio() override;

	private:
		void on_paint(wxPaintEvent& event);

		wxString title;
		CThemeIntroLogo themeFont;
	};
}
