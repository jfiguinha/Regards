#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"

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
			bool InsertPhotoProcessing(const wxString &path);
			bool InsertPhotoCategorie(const int &numPhoto, const int &categorie);
			bool DeletePhotoProcessing(const wxString &path);
			bool DeletePhotoProcessingDatabase();
			bool DeletePhotoCategorie(const int &numPhoto);
			bool DeletePhotoCategorieDatabase();
		private:

			int type;
			int TraitementResult(CSqlResult * sqlResult);
			vector<wxString> listPhoto;
		};
	}
}
