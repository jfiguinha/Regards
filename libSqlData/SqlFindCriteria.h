#pragma once
#include <Criteria.h>
#include "SqlExecuteRequest.h"
#include "SqlResult.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlFindCriteria : public CSqlExecuteRequest
		{
		public:
			CSqlFindCriteria();
			~CSqlFindCriteria();
			bool SearchUniqueCriteria(CriteriaVector * criteriaVector, const int64_t & numFolder, const int64_t &numCatalog);
			bool SearchCriteria(CriteriaVector * criteriaVector, const int64_t & numCategorie, const int64_t &numCatalog);
			bool SearchCriteriaAlone(CriteriaVector * criteriaVector);
			bool SearchCriteria(CriteriaVector * criteriaVector, const wxString &key, const int64_t &numCategorie, const int64_t &numCatalog);
			bool SearchCriteria(CriteriaVector * criteriaVector, const int64_t & numPhoto);
			bool SearchCriteria(CriteriaVector * criteriaVector, const int64_t & numCategorie, const int64_t &numCatalog, const int64_t &numFolder);
		private:

			int TraitementResult(CSqlResult * sqlResult);
			CriteriaVector * m_criteriaVector;
		};
	}
}

