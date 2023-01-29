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
			~CSqlPhotosWithoutThumbnail() override;
			void GetPhotoList(vector<wxString>* photoList, int nbElement = 20);
			int GetPhotoElement();

		private:
			bool IsPathFind(const wxString& photo);

			int TraitementResult(CSqlResult* sqlResult) override;
			int typeResult;
			int priority;
			int nbElement = 0;
			vector<wxString>* photoList;
			wxString fullpath;
		};
	}
}
