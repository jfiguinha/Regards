#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"
#ifdef __APPLE__
#include <AppleSecurityScopeData.h>
#endif

namespace Regards
{
	namespace Sqlite
	{
		class CSqlFolderCatalog : public CSqlExecuteRequest
		{
		public:
			CSqlFolderCatalog();
			~CSqlFolderCatalog();
#ifdef __APPLE__
			bool InsertFolderCatalog(const int64_t &numCatalog, const wxString & folderPath, CAppleSecurityScopeData * appleSecurityScopeData);
            CAppleSecurityScopeData * GetDataSecurityScope(const wxString & folderPath);
#else
            bool InsertFolderCatalog(const int64_t &numCatalog, const wxString & folderPath);
#endif
			int64_t GetFolderCatalogId(const int64_t &numCatalog, const wxString & folderPath);
			int64_t GetOrInsertFolderCatalog(const int64_t &numCatalog, const wxString & folderPath);
			bool DeleteFolder(const int64_t &numFolder);
			bool DeleteCatalog(const int64_t &numCatalog);
			wxString GetFolderCatalogPath(const int64_t &numFolder);

		private:

			int TraitementResult(CSqlResult * sqlResult);
#ifdef __APPLE__
            CAppleSecurityScopeData * appleSecurityScopeData = nullptr;
#endif
			wxString folderPath;
			int64_t numCatalogId;
			int64_t numFolderCatalogId;
            int typeResult = 1;
		};
	}
}

