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
		class CSqlFindLocalisation : public CSqlExecuteRequest
		{
		public:
			CSqlFindLocalisation();
			~CSqlFindLocalisation();
			bool SearchUniqueCriteria(vector<wxString> * localisationVector, const wxString &day, const wxString &month, const wxString &year);

		private:

			int TraitementResult(CSqlResult * sqlResult);
			vector<wxString> * localisationVector;
		};
	}
}

