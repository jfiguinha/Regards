#pragma once
#include <PhotoCatalog.h>
#include "SqlExecuteRequest.h"
#include "SqlResult.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlFindCatalog : public CSqlExecuteRequest
		{
		public:
			CSqlFindCatalog();
			~CSqlFindCatalog();

			bool GetCatalog(CatalogVector * catalogVector);

		private:

			int TraitementResult(CSqlResult * sqlResult);
			CatalogVector * m_catalogVector;
		};
	}
}

