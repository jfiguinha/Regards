#pragma once
#include <string>
using namespace std;

namespace Regards
{
	namespace Explorer
	{
		class CExplorerTheme;

		class CExplorerThemeInit
		{
		public:
			CExplorerThemeInit();
			~CExplorerThemeInit();

			static CExplorerTheme * getInstance();
			static void Initialize(CExplorerTheme * param);

		private:

			static CExplorerTheme *_singleton;
		};
	}
}
