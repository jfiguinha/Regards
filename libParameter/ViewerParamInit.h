#pragma once

namespace Regards
{
	namespace Viewer
	{
		class CMainParam;

		class CMainParamInit
		{
		public:
			CMainParamInit();
			~CMainParamInit();

			static CMainParam * getInstance();
			static void Initialize(CMainParam * param);

		private:

			static CMainParam *_singleton;
		};
	}
}

