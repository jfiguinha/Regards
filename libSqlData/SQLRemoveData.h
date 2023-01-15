#pragma once
#include <Criteria.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSQLRemoveData
		{
		public:
			CSQLRemoveData();
			~CSQLRemoveData();

			static bool DeleteCatalog(const int& numCatalog);
			static bool DeleteFolder(const int& numFolder);
			static bool DeleteListPhoto(const vector<int>& listPhoto, CriteriaVector* criteriaVector);
			static bool DeletePhoto(const int& idPhoto);
			static bool DeleteFaceDatabase();
		};
	}
}
