#pragma once

namespace Regards
{
	namespace Viewer
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
