#pragma once
#include <SqlLib.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSqlLibResource : public CSqlLib
		{
		public:
			CSqlLibResource(const bool &readOnly, const bool &load_inmemory);
			~CSqlLibResource();

			bool InitDatabase(const wxString &lpFilename);

		};
	}
}

