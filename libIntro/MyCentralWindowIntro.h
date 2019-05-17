#pragma once
#include "infoAbout.h"
#include "TitleIntro.h"
#include <TreeWindow.h>
#include <ScrollbarWnd.h>
#include <TreeWithScrollbar.h>

using namespace Regards::Window;

namespace Regards
{
	namespace Introduction
	{
		class CMyCentralWindowIntro : public CTreeWithScrollbar
		{
		public:
			CMyCentralWindowIntro(wxWindow* parent, wxWindowID id, const CThemeScrollBar & themeScroll, const CThemeTree & theme);
			~CMyCentralWindowIntro();

		private:

			CInfoAbout * infoAbout;
		};
	}
}
