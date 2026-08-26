#pragma once

namespace Regards
{
	namespace Scanner
	{
		class CMainParam;

		class CMainParamInit
		{
		public:

			static CMainParam* getInstance();
			

		private:
			static void Initialize();
			static std::unique_ptr<CMainParam> _singleton;
		};
	}
}
