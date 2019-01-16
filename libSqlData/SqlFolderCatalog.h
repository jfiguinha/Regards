#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"


namespace Regards
{
	namespace Sqlite
	{
		class CSqlFolderCatalog : public CSqlExecuteRequest
		{
		public:
			CSqlFolderCatalog();
			~CSqlFolderCatalog();
            bool InsertFolderCatalog(const int64_t &numCatalog, const wxString & folderPath);
			int64_t GetFolderCatalogId(const int64_t &numCatalog, const wxString & folderPath);
			int64_t GetOrInsertFolderCatalog(const int64_t &numCatalog, const wxString & folderPath);
			bool DeleteFolder(const int64_t &numFolder);
			bool DeleteCatalog(const int64_t &numCatalog);
			wxString GetFolderCatalogPath(const int64_t &numFolder);

		private:

			int TraitementResult(CSqlResult * sqlResult);
			wxString folderPath;
			int64_t numCatalogId;
			int64_t numFolderCatalogId;
            int typeResult;
		};
	}
}

