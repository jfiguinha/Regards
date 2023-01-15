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
			~CSqlFindYear() override;
			bool SearchUniqueCriteria(vector<wxString>* yearVector);

		private:
			int TraitementResult(CSqlResult* sqlResult) override;
			vector<wxString>* yearVector;
		};
	}
}
