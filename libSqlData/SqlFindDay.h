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
		class CSqlFindDay : public CSqlExecuteRequest
		{
		public:
			CSqlFindDay();
			~CSqlFindDay();
			bool SearchUniqueCriteria(vector<wxString> * dayVector, const wxString &month, const wxString &year);

		private:

			int TraitementResult(CSqlResult * sqlResult);
			vector<wxString> * dayVector;
		};
	}
}

