#pragma once
#include "SqlExecuteRequest.h"
#include "SqlResult.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlPhotosWithoutThumbnail : public CSqlExecuteRequest
		{
		public:
			CSqlPhotosWithoutThumbnail();
			~CSqlPhotosWithoutThumbnail();
			void GetPhotoList(vector<wxString> * photoList);
			void InsertPhotoPriority(const wxString & photoPath);
			void InsertProcessStart(const wxString & photoPath);
			void GeneratePhotoList();
			void UpdatePhotoList();
		private:
            void UpdateVideoList();
			int TraitementResult(CSqlResult * sqlResult);
			int typeResult;
			int priority;
			vector<wxString> *  photoList;
		};
	}
}
