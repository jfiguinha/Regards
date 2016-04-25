#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlCriteria : public CSqlExecuteRequest
		{
		public:
			CSqlCriteria(CSqlLib * _sqlLibTransaction = nullptr, const bool &useTransaction = 0);
			~CSqlCriteria();

			int64_t GetCriteriaId(const int &numCriteria, const int &numFolder);
            int64_t GetCriteriaIdByCategorie(const int &numPhoto, const int &numCategorie);
			int64_t GetOrInsertCriteriaId(const int64_t &numCatalog, const int64_t &numCategorie, const wxString & libelle, bool &isNew);
			bool InsertCriteria(const int64_t &numCatalog, const int64_t &numCategorie, const wxString & libelle);
			int64_t GetCriteriaId(const int64_t &numCatalog, const int64_t &numCategorie, const wxString & libelle);
			bool DeletePhotoCriteria(const int64_t &numCatalog, const int64_t &numCriteria);
			int64_t GetNumCategorieId(const int64_t &numCatalog, const wxString & libelle);
			bool DeleteCriteriaFolder(const int64_t &numFolder);
			bool DeleteCriteriaCatalog(const int64_t &numCatalog);
			bool DeleteCriteriaAlone();
            void RemoveUnusedCriteria();
            
		private:

			int TraitementResult(CSqlResult * sqlResult);
			int64_t criteriaId = 0;

		};
	}
}

