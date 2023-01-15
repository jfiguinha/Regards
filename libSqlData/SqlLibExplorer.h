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
			CSqlLibExplorer(const bool& readOnly, const wxString& libelleNotGeo, const bool& load_inmemory);
			~CSqlLibExplorer() override;

			bool InitDatabase(const wxString& lpFilename) override;
			bool CheckVersion(const wxString& lpFilename) override;

		private:
			wxString libelleNotGeo;
			bool CreateDatabase(const wxString& databasePath, const bool& load_inmemory);
			void LoadAndRotate(const wxString& filePath, const int& rotate);
		};
	}
}
