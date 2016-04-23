#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlCatalog : public CSqlExecuteRequest
		{
		public:
			CSqlCatalog();
			~CSqlCatalog();
			bool InsertCatalog(const wxString & libelle);
			bool UpdateCatalog(const int64_t &numCatalog, const wxString & libelle);
			int64_t GetCatalogId(const wxString & libelle);
			bool DeleteCatalog(const int64_t &numCatalog);
			wxString GetCatalogLibelle(const int64_t &numCatalog);

		private:

			int TraitementResult(CSqlResult * sqlResult);

			wxString libelle;
			int64_t numCatalogId;
		};
	}
}
