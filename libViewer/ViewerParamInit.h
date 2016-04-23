#pragma once
#include <string>
using namespace std;

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

