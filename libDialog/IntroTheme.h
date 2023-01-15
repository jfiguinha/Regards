#pragma once
#include <ThemeParam.h>

namespace Regards::Introduction
{
	class CIntroTheme final : public CThemeParam
	{
	public:
		CIntroTheme();

		~CIntroTheme() override;

		void GetAboutTexteTheme(CThemeIntroLogo* theme) const;

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
