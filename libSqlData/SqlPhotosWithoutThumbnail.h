#pragma once
#include "SqlExecuteRequest.h"

namespace Regards
{
	namespace Sqlite
	{
		class CSqlResult;

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
			void UpdateVideoList();
		private:

			bool IsPathFind(const wxString & photo);
            
			int TraitementResult(CSqlResult * sqlResult);
			int typeResult;
			int priority;
			vector<wxString> *  photoList;
			wxString fullpath;
		};
	}
}
