#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"
#include <string>
#include <vector>
using namespace std;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlFindYear : public CSqlExecuteRequest
		{
		public:
			CSqlFindYear();
			~CSqlFindYear();
			bool SearchUniqueCriteria(vector<wxString> * yearVector);

		private:

			int TraitementResult(CSqlResult * sqlResult);
			vector<wxString> * yearVector;
		};
	}
}

