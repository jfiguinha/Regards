#pragma once
#include <string>
using namespace std;

namespace Regards
{
	namespace Viewer
	{
		class CViewerTheme;

		class CViewerThemeInit
		{
		public:
			CViewerThemeInit();
			~CViewerThemeInit();

			static CViewerTheme * getInstance();
			static void Initialize(CViewerTheme * param);

		private:

			static CViewerTheme *_singleton;
		};
	}
}

