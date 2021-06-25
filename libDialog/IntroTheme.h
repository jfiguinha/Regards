#pragma once
#include <ThemeParam.h>

namespace Regards
{
	namespace Introduction
	{
		class CIntroTheme final : public CThemeParam
		{
		public:
			CIntroTheme();

			~CIntroTheme() override;

			auto GetAboutTexteTheme(CThemeIntroLogo* theme) const -> void;

		private:
			void LoadTheme() override
			{
			}

			void SaveTheme() override
			{
			}

			CThemeIntroLogo themeIntroLogo;
		};
	}
}
