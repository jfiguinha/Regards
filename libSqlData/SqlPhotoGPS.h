#pragma once
#include "SqlExecuteRequest.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

		class CSqlPhotoGPS : public CSqlExecuteRequest
		{
		public:
			CSqlPhotoGPS(CSqlLib* _sqlLibTransaction = nullptr, const bool& useTransaction = false);
			~CSqlPhotoGPS() override;
			bool InsertPhoto(const int& numPhoto, const wxString& filepath, const int& numFolderId);
			bool DeletePhoto(const int64_t& numPhoto);
			int GetFirstPhoto(int& numPhoto, wxString& filepath, int& numFolderId);
			bool DeleteListOfPhoto(const vector<wxString>& listPhoto);
		private:
			int TraitementResult(CSqlResult* sqlResult) override;
			int numPhoto;
			int numFolderId;
			wxString filepath;
			int nbResult;
		};
	}
}
