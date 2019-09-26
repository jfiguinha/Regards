#pragma once

namespace Regards
{
	namespace Viewer
	{
		class CScannerParam;

		class CScannerParamInit
		{
		public:
			CScannerParamInit();
			~CScannerParamInit();

			static CScannerParam * getInstance();
			static void Initialize(CScannerParam * param);

		private:

			static CScannerParam *_singleton;
		};
	}
}

