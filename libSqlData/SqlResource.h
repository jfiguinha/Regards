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
			CSqlResource(CSqlLib* _sqlLibTransaction = nullptr, const bool& useTransaction = false);
			~CSqlResource() = default;

			vector<wxString> GetSavePictureFormat();
			vector<wxString> GetSavePictureExtension();

			wxString GetVectorFromFile(const wxString& idName);
			wxString GetLibelle(const wxString& idName, const int& idLang);
			int GetExtensionId(const wxString& extension);
			wxString GetOpenCLUcharFromFile(const wxString& idName);
			wxString GetOpenGLFromFile(const wxString& idName);

		private:

			int TraitementResult(CSqlResult* sqlResult);

			int typeResult;

			int id = 0;
			wxString text;
			vector<wxString> list;
		};
	}
}
