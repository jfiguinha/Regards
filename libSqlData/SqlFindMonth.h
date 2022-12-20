#pragma once
#include "SqlExecuteRequest.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

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

