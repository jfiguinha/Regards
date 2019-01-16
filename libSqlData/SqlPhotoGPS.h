#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"


namespace Regards
{
	namespace Sqlite
	{
		class CSqlPhotoGPS : public CSqlExecuteRequest
		{
		public:
			CSqlPhotoGPS(CSqlLib * _sqlLibTransaction = nullptr, const bool &useTransaction = 0);
			~CSqlPhotoGPS();
			bool InsertPhoto(const int &numPhoto, const wxString & filepath, const int & numFolderId);
			bool DeletePhoto(const int64_t &numPhoto);
			int GetFirstPhoto(int & numPhoto, wxString & filepath, int & numFolderId);

		private:

			int TraitementResult(CSqlResult * sqlResult);
			int numPhoto;
            int numFolderId;
            wxString filepath;
            int nbResult;
		};
	}
}

