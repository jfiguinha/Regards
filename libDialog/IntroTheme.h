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

		private:

			void LoadTheme(){};
			void SaveTheme(){};

			CThemeIntroLogo themeIntroLogo;
		};
	}
}
