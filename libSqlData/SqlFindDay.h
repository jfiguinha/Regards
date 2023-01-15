#pragma once
#include "SqlExecuteRequest.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlFindDay : public CSqlExecuteRequest
		{
		public:
			CSqlFindDay();
			~CSqlFindDay() override;
			bool SearchUniqueCriteria(vector<wxString>* dayVector, const wxString& month, const wxString& year);

		private:
			int TraitementResult(CSqlResult* sqlResult) override;
			vector<wxString>* dayVector;
		};
	}
}
