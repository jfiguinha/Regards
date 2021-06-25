#pragma once
#include "SqlExecuteRequest.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlPhotoCategorieUsenet : public CSqlExecuteRequest
		{
		public:
			CSqlPhotoCategorieUsenet();
			~CSqlPhotoCategorieUsenet() override;
			vector<wxString> GetPhotoListTreatment();
			bool InsertPhotoProcessing(const wxString &path);
			bool InsertPhotoCategorie(const int &numPhoto, const int &categorie, const wxString &label);
			bool DeletePhotoProcessing(const wxString &path);
			bool DeletePhotoProcessingDatabase();
		private:

			int type;
			int TraitementResult(CSqlResult * sqlResult);
			vector<wxString> listPhoto;

		};
	}
}
