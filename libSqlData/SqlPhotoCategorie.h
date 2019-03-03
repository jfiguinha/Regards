#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"
#include <PhotoCategorie.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSqlPhotoCategorie : public CSqlExecuteRequest
		{
		public:
			CSqlPhotoCategorie();
			~CSqlPhotoCategorie();
			bool LoadPhotoCategorie(PhotoCategorieVector * photoCategorieVector, const int &numLangue);

		private:

			int TraitementResult(CSqlResult * sqlResult);

			PhotoCategorieVector * m_PhotoCategorieVector;
		};
	}
}

