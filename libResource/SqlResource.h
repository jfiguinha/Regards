#pragma once
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
            int GetExtensionId(const wxString &extension);
			void InsertBitmap(const wstring &idName, const wstring &mimeType, const wstring &filename, const bool &flip = false);
			void InsertText(const wstring &idName, const wstring &mimeType, const wstring &filename);
			void InsertLibelle(const wstring &idName, const wstring &libelle, const int &lang);
			void InsertVector(const wstring &idName, const wstring &filename);

		private:

			string readFileBytes(const string &name);
			int TraitementResultExif(CSqlResult * sqlResult);
            int TraitementResultVector(CSqlResult * sqlResult);
			int TraitementResultBitmap(CSqlResult * sqlResult);
			int TraitementResultText(CSqlResult * sqlResult);
			int TraitementResultLibelle(CSqlResult * sqlResult);
            int TraitementResultExtension(CSqlResult * sqlResult);
			int TraitementResult(CSqlResult * sqlResult);
			int typeResult;
			CPictureData * memFile;
			wxString text;
			wxString libelle;
            string test;
            int id;
		};
	}
}

