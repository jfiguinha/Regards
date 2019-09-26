#pragma once

namespace Regards
{
	namespace Viewer
	{
		class CScannerTheme;

		class CScannerThemeInit
		{
		public:
			CScannerThemeInit();
			virtual ~CScannerThemeInit();

			static CScannerTheme * getInstance();
			static void Initialize(CScannerTheme * param);
            static void SaveTheme();
		private:

			static CScannerTheme *_singleton;
		};
	}
}

