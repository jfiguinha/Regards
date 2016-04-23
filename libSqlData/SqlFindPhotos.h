#pragma once
#include <Photos.h>
#include "SqlExecuteRequest.h"
#include "SqlResult.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlFindPhotos : public CSqlExecuteRequest
		{
		public:
			CSqlFindPhotos();
			~CSqlFindPhotos();
			bool SearchPhotos(PhotosVector * photosVector);
			bool SearchPhotos(vector<int> * listPhoto);
			bool SearchPhotos(PhotosVector * photosVector, const wxString &libelleCriteria);
			bool SearchPhotos(PhotosVector * photosVector, const wxString &localisation, const wxString &libelleCriteria);
			bool SearchPhotos(PhotosVector * photosVector, const int &numCategorie, const int &numCatalog);
			bool SearchPhotos(PhotosVector * photosVector, const int &numCatalog);
			bool SearchPhotos(const int &numCatalog, vector<int> listFolder, vector<int> listCriteriaNotIn);
		private:

			int TraitementResultPhoto(CSqlResult * sqlResult);
			int TraitementResultNumPhoto(CSqlResult * sqlResult);
			bool DeleteAllInSearchPhotos();
			wxString GetSearchSQL(vector<int> list);
			int TraitementResult(CSqlResult * sqlResult);
			PhotosVector * m_photosVector;
			vector<int> * m_listPhoto;
			int typeResult = 0;
		};
	}
}

