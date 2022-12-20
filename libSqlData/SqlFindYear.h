#pragma once
#include "SqlExecuteRequest.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

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

