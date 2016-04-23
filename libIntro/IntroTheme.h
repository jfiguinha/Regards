#pragma once
#include <ThemeParam.h>
using namespace rapidxml;
using namespace std;

namespace Regards
{
	namespace Introduction
	{
		class CIntroTheme : public CThemeParam
		{
		public:
			CIntroTheme();
			~CIntroTheme(){};
			void GetAboutTexteTheme(CThemeIntroLogo * theme);
			void GetAboutTreeControlTheme(CThemeTree * theme);
			void GetScrollAboutTheme(CThemeScrollBar * theme);

		private:

			void LoadTheme(){};
			void SaveTheme(){};

			void InitAboutTree();
			void InitAboutScrollbar();

			CThemeTree themeAbout;
			CThemeScrollBar themeAboutScroll;
			CThemeIntroLogo themeIntroLogo;
		};
	}
}
