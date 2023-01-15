#pragma once
#include "SqlExecuteRequest.h"
#include <Country.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlCountry : public CSqlExecuteRequest
		{
		public:
			CSqlCountry();
			~CSqlCountry() override;
			bool GetCountry(CountryVector* countryVector);

		private:
			int TraitementResult(CSqlResult* sqlResult) override;
			CountryVector* m_countryVector;
		};
	}
}
