#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"
#include <Country.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSqlCountry : public CSqlExecuteRequest
		{
		public:
			CSqlCountry();
			~CSqlCountry();
			bool GetCountry(CountryVector * countryVector);

		private:

			int TraitementResult(CSqlResult * sqlResult);
			CountryVector * m_countryVector;
		};
	}
}

