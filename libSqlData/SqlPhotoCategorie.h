#pragma once
#include "SqlExecuteRequest.h"
#include <PhotoCategorie.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlPhotoCategorie : public CSqlExecuteRequest
		{
		public:
			CSqlPhotoCategorie();
			~CSqlPhotoCategorie() override;
			bool LoadPhotoCategorie(PhotoCategorieVector * photoCategorieVector, const int &numLangue);

		private:

			int TraitementResult(CSqlResult * sqlResult);

			PhotoCategorieVector * m_PhotoCategorieVector;
		};
	}
}

