#pragma once
#include <SqlLib.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlLibExplorer : public CSqlLib
		{
		public:
			CSqlLibExplorer(const bool &readOnly, const bool &load_inmemory);
			~CSqlLibExplorer();

			bool InitDatabase(const wxString &lpFilename);
            bool CheckVersion(const wxString &lpFilename);
		private:
			
			bool CreateDatabase(const wxString &databasePath, const bool &load_inmemory);
		};
	}
}

