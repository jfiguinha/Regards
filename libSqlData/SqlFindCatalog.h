#pragma once
#include <PhotoCatalog.h>
#include "SqlExecuteRequest.h"

namespace Regards
{
	class CSqlResult;

	namespace Sqlite
	{
		class CSqlFindCatalog : public CSqlExecuteRequest
		{
		public:
			CSqlFindCatalog();
			~CSqlFindCatalog() override;

			bool GetCatalog(CatalogVector* catalogVector);

		private:
			int TraitementResult(CSqlResult* sqlResult) override;
			CatalogVector* m_catalogVector;
		};
	}
}
