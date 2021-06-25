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
			CSqlLibExplorer(const bool &readOnly, const wxString &libelleNotGeo, const bool &load_inmemory);
			~CSqlLibExplorer() override;

			auto InitDatabase(const wxString& lpFilename) -> bool override;
			auto CheckVersion(const wxString& lpFilename) -> bool override;
		private:
			wxString libelleNotGeo;
			bool CreateDatabase(const wxString &databasePath, const bool &load_inmemory);
		};
	}
}

