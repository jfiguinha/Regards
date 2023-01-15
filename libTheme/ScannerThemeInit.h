#pragma once
namespace Regards
{
	namespace Scanner
	{
		class CMainTheme;

		class CMainThemeInit
		{
		public:
			CMainThemeInit();
			virtual ~CMainThemeInit();

			static CMainTheme* getInstance();
			static void Initialize(CMainTheme* param);
			static void SaveTheme();

		private:
			static CMainTheme* _singleton;
		};
	}
}
