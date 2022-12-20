#pragma once
#include "SqlExecuteRequest.h"
namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlCatalog final : public CSqlExecuteRequest
		{
		public:
			CSqlCatalog();
			~CSqlCatalog();
			bool InsertCatalog(const wxString& libelle);
			bool UpdateCatalog(const int64_t& numCatalog, const wxString& libelle);
			int64_t GetCatalogId(const wxString& libelle);
			bool DeleteCatalog(const int64_t& numCatalog);
			wxString GetCatalogLibelle(const int64_t& numCatalog);

		private:

			int TraitementResult(CSqlResult* sqlResult) override;

			wxString libelle;
			int64_t numCatalogId;
		};
	}
}
