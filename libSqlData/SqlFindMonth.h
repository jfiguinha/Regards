#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlFindMonth : public CSqlExecuteRequest
		{
		public:
			CSqlFindMonth();
			~CSqlFindMonth();
			bool SearchUniqueCriteria(vector<wxString> * monthVector, const wxString &year);

		private:

			int TraitementResult(CSqlResult * sqlResult);
			vector<wxString> * monthVector;
		};
	}
}

