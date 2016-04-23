#pragma once
#include <WindowMain.h>
using namespace Regards::Window;

namespace Regards
{
	namespace Introduction
	{
		class CTitleIntro : public CWindowMain
		{
		public:
			CTitleIntro(wxWindow* parent, wxWindowID id, const CThemeIntroLogo & theme);
			~CTitleIntro();

			void SetTitle(const wxString &title);
			int GetHeight();


		private:
			void Resize();
			void OnPaint(wxPaintEvent& event);

			wxString title;
			CThemeIntroLogo themeFont;
		};
	}
}

