#pragma once

namespace Regards
{
	namespace Viewer
	{
		class CViewerParam;

		class CViewerParamInit
		{
		public:
			CViewerParamInit();
			~CViewerParamInit();

			static CViewerParam * getInstance();
			static void Initialize(CViewerParam * param);

		private:

			static CViewerParam *_singleton;
		};
	}
}

