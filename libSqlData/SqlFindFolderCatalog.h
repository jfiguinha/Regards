#pragma once
#include <FolderCatalog.h>
#include "SqlExecuteRequest.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlFindFolderCatalog : public CSqlExecuteRequest
		{
		public:
			CSqlFindFolderCatalog();
			~CSqlFindFolderCatalog() override;
			bool GetFolderCatalog(FolderCatalogVector* catalogfolderVector, const int& numCatalog);
			int GetNbFolderCatalog(const int& numCatalog);

		private:
			int TraitementResult(CSqlResult* sqlResult) override;
			FolderCatalogVector* m_catalogFolderVector;
			int nbCatalog;
			int typeRequest;
		};
	}
}
