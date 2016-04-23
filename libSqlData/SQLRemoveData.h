#pragma once
#include <Criteria.h>
#include <vector>
using namespace std;

namespace Regards
{
	namespace Sqlite
	{
		class CSQLRemoveData
		{
		public:
			CSQLRemoveData();
			~CSQLRemoveData();

			static bool DeleteCatalog(const int &numCatalog);
			static bool DeleteFolder(const int &numFolder);
			static bool DeleteListPhoto(const vector<int> &listPhoto, CriteriaVector * criteriaVector);
		};
	}
}

