#pragma once
#include <string>
using namespace std;
namespace Regards
{
	namespace Explorer
	{
		class CExplorerParam;

		class CExplorerParamInit
		{
		public:
			CExplorerParamInit();
			~CExplorerParamInit();

			static CExplorerParam * getInstance();
			static void Initialize(CExplorerParam * param);

		private:

			static CExplorerParam *_singleton;
		};
	}
}
