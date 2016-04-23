#pragma once
#include "SqlExecuteRequest.h"
#include <ListCriteriaPhoto.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSqlPhotoCriteria : public CSqlExecuteRequest
		{
		public:
			CSqlPhotoCriteria();
			~CSqlPhotoCriteria();

			bool InsertPhotoListCriteria(const CListCriteriaPhoto &listPhotoCriteria, bool &isNew, bool criteriaUpdate = true);
			bool InsertPhotoCriteria(const int64_t &numPhoto, const int64_t &numCriteria);
			bool DeletePhoto(const int64_t &numPhoto);
			bool DeleteCriteria(const int64_t &numCriteria);
			bool DeleteFolderCriteria(const int64_t &numFolder);
			bool DeleteCatalogCriteria(const int64_t &numCatalog);

		private:

			int TraitementResult(CSqlResult * sqlResult){ return false; };
		};
	}
}

