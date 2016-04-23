#pragma once
#include <string>
#include "SqlExecuteRequest.h"
#include "SqlResult.h"
#include "PictureData.h"


namespace Regards
{
	namespace Sqlite
	{
		class CSqlResource : public CSqlExecuteRequest
		{
		public:
			CSqlResource(CSqlLib * _sqlLibTransaction = nullptr, const bool &useTransaction = 0);
			~CSqlResource();

			CPictureData * GetBitmap(const wxString &idName);
			wxString GetText(const wxString &idName);
            wxString GetVector(const wxString &idName);
			wxString GetLibelle(const wxString &idName, const int &idLang);
			wxString GetExifLibelle(const wxString &idName);

		private:

			int TraitementResultExif(CSqlResult * sqlResult);
            int TraitementResultVector(CSqlResult * sqlResult);
			int TraitementResultBitmap(CSqlResult * sqlResult);
			int TraitementResultText(CSqlResult * sqlResult);
			int TraitementResultLibelle(CSqlResult * sqlResult);
			int TraitementResult(CSqlResult * sqlResult);
			int typeResult;
			CPictureData * memFile;
			wxString text;
			wxString libelle;
            string test;
		};
	}
}

