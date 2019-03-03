#pragma once

namespace Regards
{
	namespace Viewer
	{
		class CViewerTheme;

		class CViewerThemeInit
		{
		public:
			CViewerThemeInit();
			virtual ~CViewerThemeInit();

			static CViewerTheme * getInstance();
			static void Initialize(CViewerTheme * param);
            static void SaveTheme();
		private:

			static CViewerTheme *_singleton;
		};
	}
}

