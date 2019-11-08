#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"
#include <PhotoCategorieUsenet.h>

namespace Regards
{
	namespace Sqlite
	{
		class CSqlPhotoCategorieUsenet : public CSqlExecuteRequest
		{
		public:
			CSqlPhotoCategorieUsenet();
			~CSqlPhotoCategorieUsenet();
			vector<wxString> GetPhotoListTreatment();
			vector<CPhotoCategorieUsenet> GetPhotoListCategorie(const int &numPhoto);
			bool InsertPhotoProcessing(const wxString &path);
			bool InsertPhotoCategorie(const int &numPhoto, const int &categorie, const wxString &label);
			bool DeletePhotoProcessing(const wxString &path);
			bool DeletePhotoProcessingDatabase();
			bool DeletePhotoCategorie(const int &numPhoto);
			bool DeletePhotoCategorieDatabase();
		private:

			int type;
			int TraitementResult(CSqlResult * sqlResult);
			vector<wxString> listPhoto;
			vector<CPhotoCategorieUsenet> listCategoriePhoto;
		};
	}
}
