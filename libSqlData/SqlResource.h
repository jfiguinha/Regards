#pragma once
#include "SqlExecuteRequest.h"

class CPictureData;

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlResource : public CSqlExecuteRequest
		{
		public:
			CSqlResource(CSqlLib * _sqlLibTransaction = nullptr, const bool &useTransaction = 0);
			~CSqlResource();

			CPictureData * GetBitmap(const wxString &idName);
			wxString GetText(const wxString& idName);
			wxString GetOpenGLShader(const wxString& idName);

            wxString GetVector(const wxString &idName);
			wxString GetLibelle(const wxString &idName, const int &idLang);
			wxString GetExifLibelle(const wxString &idName);
            int GetExtensionId(const wxString &extension);
			void InsertBitmap(const wstring &idName, const wstring &mimeType, const wstring &filename, const bool &flip = false);
			void InsertOpenGLShader(const wstring &idName, const wstring &mimeType, const wstring &filename);
			void InsertOpenCLFloat(const wstring& idName, const wstring& mimeType, const wstring& filename);
			void InsertOpenCLUchar(const wstring& idName, const wstring& mimeType, const wstring& filename);
			void InsertLibelle(const wstring &idName, const wstring &libelle, const int &lang);
			void InsertVector(const wstring &idName, const wstring &filename);
			wxString GetOpenCLFloatFromFile(const wxString& idName);
			wxString GetOpenCLUcharFromFile(const wxString& idName);
			wxString GetOpenGLFromFile(const wxString& idName);

		private:
			wxString GetOpenCLFloat(const wxString& idName);
			wxString GetOpenCLUchar(const wxString& idName);
			wxString readFileBytes(const wxString & name);
			int TraitementResultExif(CSqlResult * sqlResult);
            int TraitementResultVector(CSqlResult * sqlResult);
			int TraitementResultBitmap(CSqlResult * sqlResult);
			int TraitementResultText(CSqlResult * sqlResult);
			int TraitementResultLibelle(CSqlResult * sqlResult);
            int TraitementResultExtension(CSqlResult * sqlResult);
			int TraitementResult(CSqlResult * sqlResult);
			int TraitementResultFilePath(CSqlResult * sqlResult);
			int typeResult;
			CPictureData * memFile;
			wxString text;
			wxString libelle;
            string test;
			wxString defaultPathSearch;
            int id;
		};
	}
}

