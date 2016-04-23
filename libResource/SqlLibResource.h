#pragma once
#include <string>
#include <SqlLib.h>
using namespace std;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlLibResource : public CSqlLib
		{
		public:
			CSqlLibResource();
			~CSqlLibResource();

			bool InitDatabase(const wxString &lpFilename);
		};
	}
}

